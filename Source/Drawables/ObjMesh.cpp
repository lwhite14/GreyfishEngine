#include "ObjMesh.h"

using std::string;
using glm::vec3;
using glm::vec2;

#include <cstdlib>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
#include <map>
#include "../Dependencies/imgui/imgui.h"
#include "../MasterTextures.h"

#include "../UserInterface/Console.h"

ObjMesh::ObjMesh()
{

}

void ObjMesh::RenderDrawable() 
{
    if (m_drawAdj) 
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES_ADJACENCY, nVerts, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    else 
    {
        TriangleMesh::RenderDrawable();
    }
}


ObjMesh* ObjMesh::Load(const char* fileName, bool center, bool genTangents)
{
    ObjMesh* mesh = new ObjMesh();

    ObjMeshData meshData;
    meshData.Load(fileName, mesh->bbox);

    // Generate normals
    meshData.GenerateNormalsIfNeeded();

    // Generate tangents?
    if (genTangents) meshData.GenerateTangents();

    // Convert to GL format
    GlMeshData glMesh;
    meshData.ToGlMesh(glMesh);

    if (center) glMesh.Center(mesh->bbox);

    // Load into VAO
    mesh->InitBuffers(
        &(glMesh.faces), &glMesh.points, &glMesh.normals,
        glMesh.texCoords.empty() ? nullptr : (&glMesh.texCoords),
        glMesh.tangents.empty() ? nullptr : (&glMesh.tangents)
    );

    std::string fileNameStr = "";
    for (char c = *fileName; c; c = *++fileName)
    {
        fileNameStr.push_back(c);
    }
    Console::AddMessage("Loaded mesh from: " + fileNameStr + ". Vertices = " + std::to_string(glMesh.points.size() / 3) + ". Triangles = " + std::to_string(glMesh.faces.size() / 3) + ".");
    Console::AddMessage(mesh->bbox.ToString());

    return mesh;
}

ObjMesh* ObjMesh::LoadWithAdjacency(const char* fileName, bool center) 
{

    ObjMesh* mesh(new ObjMesh());

    ObjMeshData meshData;
    meshData.Load(fileName, mesh->bbox);

    // Generate normals
    meshData.GenerateNormalsIfNeeded();

    // Convert to GL format
    GlMeshData glMesh;
    meshData.ToGlMesh(glMesh);

    if (center) glMesh.Center(mesh->bbox);

    mesh->m_drawAdj = true;
    glMesh.ConvertFacesToAdjancencyFormat();

    // Load into VAO
    mesh->InitBuffers(
        &(glMesh.faces), &glMesh.points, &glMesh.normals,
        glMesh.texCoords.empty() ? nullptr : (&glMesh.texCoords),
        glMesh.tangents.empty() ? nullptr : (&glMesh.tangents)
    );

    cout << "Loaded mesh from: " << fileName
        << " vertices = " << (glMesh.points.size() / 3)
        << " triangles = " << (glMesh.faces.size() / 3) << endl;

    return mesh;
}

void ObjMesh::ObjMeshData::Load(const char* fileName, Aabb& bbox) {
    ifstream objStream(fileName, std::ios::in);

    if (!objStream) {
        cerr << "Unable to open OBJ file: " << fileName << endl;
        exit(1);
    }

    bbox.Reset();
    string line, token;
    getline(objStream, line);
    while (!objStream.eof()) {
        // Remove comment if it exists
        size_t pos = line.find_first_of("#");
        if (pos != std::string::npos) {
            line = line.substr(0, pos);
        }

        if (line.length() > 0) {
            istringstream lineStream(line);

            lineStream >> token;

            if (token == "v") {
                float x, y, z;
                lineStream >> x >> y >> z;
                glm::vec3 p(x, y, z);
                points.push_back(p);
                bbox.Add(p);
            }
            else if (token == "vt") {
                // Process texture coordinate
                float s, t;
                lineStream >> s >> t;
                texCoords.push_back(vec2(s, t));
            }
            else if (token == "vn") {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.push_back(vec3(x, y, z));
            }
            else if (token == "f") {
                std::vector<std::string> parts;
                while (lineStream.good()) {
                    std::string s;
                    lineStream >> s;
                    parts.push_back(s);
                }

                // Triangulate as a triangle fan
                if (parts.size() > 2) {
                    ObjVertex firstVert(parts[0], this);
                    for (int i = 2; i < parts.size(); i++) {
                        faces.push_back(firstVert);
                        faces.push_back(ObjVertex(parts[i - 1], this));
                        faces.push_back(ObjVertex(parts[i], this));
                    }
                }
            }
        }
        getline(objStream, line);
    }
    objStream.close();
}

void ObjMesh::GlMeshData::Center(Aabb& bbox) {
    if (points.empty()) return;

    // Center of the AABB
    glm::vec3 center = 0.5f * (bbox.max + bbox.min);

    // Translate center of the AABB to the origin
    for (int i = 0; i < points.size(); i += 3) {
        points[i] -= center.x;
        points[i + 1] -= center.y;
        points[i + 2] -= center.z;
    }

    // Update bbox
    bbox.max = bbox.max - center;
    bbox.min = bbox.min - center;
}

ObjMesh::ObjMeshData::ObjVertex::ObjVertex(std::string& vertString, ObjMeshData* mesh) : pIdx(-1), nIdx(-1), tcIdx(-1) {
    size_t slash1, slash2;
    slash1 = vertString.find("/");
    pIdx = std::stoi(vertString.substr(0, slash1));
    if (pIdx < 0) pIdx += mesh->points.size();
    else pIdx--;
    if (slash1 != string::npos) {
        slash2 = vertString.find("/", slash1 + 1);
        if (slash2 > slash1 + 1) {
            tcIdx = std::stoi(vertString.substr(slash1 + 1, slash2 - slash1 - 1));
            if (tcIdx < 0) tcIdx += mesh->texCoords.size();
            else tcIdx--;
        }
        nIdx = std::stoi(vertString.substr(slash2 + 1));
        if (nIdx < 0) nIdx += mesh->normals.size();
        else nIdx--;
    }
}

void ObjMesh::ObjMeshData::GenerateNormalsIfNeeded() 
{
    if (normals.size() != 0) return;

    normals.resize(points.size());

    for (GLuint i = 0; i < faces.size(); i += 3) 
    {
        const vec3& p1 = points[faces[i].pIdx];
        const vec3& p2 = points[faces[i + 1].pIdx];
        const vec3& p3 = points[faces[i + 2].pIdx];

        vec3 a = p2 - p1;
        vec3 b = p3 - p1;
        vec3 n = glm::normalize(glm::cross(a, b));

        normals[faces[i].pIdx] += n;
        normals[faces[i + 1].pIdx] += n;
        normals[faces[i + 2].pIdx] += n;

        // Set the normal index to be the same as the point index
        faces[i].nIdx = faces[i].pIdx;
        faces[i + 1].nIdx = faces[i + 1].pIdx;
        faces[i + 2].nIdx = faces[i + 2].pIdx;
    }

    for (GLuint i = 0; i < normals.size(); i++) 
    {
        normals[i] = glm::normalize(normals[i]);
    }
}

void ObjMesh::ObjMeshData::GenerateTangents() 
{
    std::vector<vec3> tan1Accum(points.size());
    std::vector<vec3> tan2Accum(points.size());
    tangents.resize(points.size());

    // Compute the tangent std::vector
    for (GLuint i = 0; i < faces.size(); i += 3)
    {
        const vec3& p1 = points[faces[i].pIdx];
        const vec3& p2 = points[faces[i + 1].pIdx];
        const vec3& p3 = points[faces[i + 2].pIdx];

        const vec2& tc1 = texCoords[faces[i].tcIdx];
        const vec2& tc2 = texCoords[faces[i + 1].tcIdx];
        const vec2& tc3 = texCoords[faces[i + 2].tcIdx];

        vec3 q1 = p2 - p1;
        vec3 q2 = p3 - p1;
        float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
        float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
        float r = 1.0f / (s1 * t2 - s2 * t1);
        vec3 tan1((t2 * q1.x - t1 * q2.x) * r,
            (t2 * q1.y - t1 * q2.y) * r,
            (t2 * q1.z - t1 * q2.z) * r);
        vec3 tan2((s1 * q2.x - s2 * q1.x) * r,
            (s1 * q2.y - s2 * q1.y) * r,
            (s1 * q2.z - s2 * q1.z) * r);
        tan1Accum[faces[i].pIdx] += tan1;
        tan1Accum[faces[i + 1].pIdx] += tan1;
        tan1Accum[faces[i + 2].pIdx] += tan1;
        tan2Accum[faces[i].pIdx] += tan2;
        tan2Accum[faces[i + 1].pIdx] += tan2;
        tan2Accum[faces[i + 2].pIdx] += tan2;
    }

    for (GLuint i = 0; i < points.size(); ++i)
    {
        const vec3& n = normals[i];
        vec3& t1 = tan1Accum[i];
        vec3& t2 = tan2Accum[i];

        // Gram-Schmidt orthogonalize
        tangents[i] = glm::vec4(glm::normalize(t1 - (glm::dot(n, t1) * n)), 0.0f);
        // Store handedness in w
        tangents[i].w = (glm::dot(glm::cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
    }
}

void ObjMesh::ObjMeshData::ToGlMesh(GlMeshData& data) 
{
    data.Clear();

    std::map<std::string, GLuint> vertexMap;
    for (auto& vert : faces) {
        auto vertStr = vert.str();
        auto it = vertexMap.find(vertStr);
        if (it == vertexMap.end()) {
            auto vIdx = data.points.size() / 3;

            auto& pt = points[vert.pIdx];
            data.points.push_back(pt.x);
            data.points.push_back(pt.y);
            data.points.push_back(pt.z);

            auto& n = normals[vert.nIdx];
            data.normals.push_back(n.x);
            data.normals.push_back(n.y);
            data.normals.push_back(n.z);

            if (!texCoords.empty()) {
                auto& tc = texCoords[vert.tcIdx];
                data.texCoords.push_back(tc.x);
                data.texCoords.push_back(tc.y);
            }

            if (!tangents.empty()) {
                // We use the point index for tangents
                auto& tang = tangents[vert.pIdx];
                data.tangents.push_back(tang.x);
                data.tangents.push_back(tang.y);
                data.tangents.push_back(tang.z);
                data.tangents.push_back(tang.w);
            }

            data.faces.push_back((GLuint)vIdx);
            vertexMap[vertStr] = (GLuint)vIdx;
        }
        else {
            data.faces.push_back(it->second);
        }
    }
}

void ObjMesh::GlMeshData::ConvertFacesToAdjancencyFormat()
{
    // Elements with adjacency info
    std::vector<GLuint> elAdj(faces.size() * 2);

    // Copy and make room for adjacency info
    for (GLuint i = 0; i < faces.size(); i += 3)
    {
        elAdj[i * 2 + 0] = faces[i];
        elAdj[i * 2 + 1] = std::numeric_limits<GLuint>::max();
        elAdj[i * 2 + 2] = faces[i + 1];
        elAdj[i * 2 + 3] = std::numeric_limits<GLuint>::max();
        elAdj[i * 2 + 4] = faces[i + 2];
        elAdj[i * 2 + 5] = std::numeric_limits<GLuint>::max();
    }

    // Find matching edges
    for (GLuint i = 0; i < elAdj.size(); i += 6)
    {
        // A triangle
        GLuint a1 = elAdj[i];
        GLuint b1 = elAdj[i + 2];
        GLuint c1 = elAdj[i + 4];

        // Scan subsequent triangles
        for (GLuint j = i + 6; j < elAdj.size(); j += 6)
        {
            GLuint a2 = elAdj[j];
            GLuint b2 = elAdj[j + 2];
            GLuint c2 = elAdj[j + 4];

            // Edge 1 == Edge 1
            if ((a1 == a2 && b1 == b2) || (a1 == b2 && b1 == a2))
            {
                elAdj[i + 1] = c2;
                elAdj[j + 1] = c1;
            }
            // Edge 1 == Edge 2
            if ((a1 == b2 && b1 == c2) || (a1 == c2 && b1 == b2))
            {
                elAdj[i + 1] = a2;
                elAdj[j + 3] = c1;
            }
            // Edge 1 == Edge 3
            if ((a1 == c2 && b1 == a2) || (a1 == a2 && b1 == c2))
            {
                elAdj[i + 1] = b2;
                elAdj[j + 5] = c1;
            }
            // Edge 2 == Edge 1
            if ((b1 == a2 && c1 == b2) || (b1 == b2 && c1 == a2))
            {
                elAdj[i + 3] = c2;
                elAdj[j + 1] = a1;
            }
            // Edge 2 == Edge 2
            if ((b1 == b2 && c1 == c2) || (b1 == c2 && c1 == b2))
            {
                elAdj[i + 3] = a2;
                elAdj[j + 3] = a1;
            }
            // Edge 2 == Edge 3
            if ((b1 == c2 && c1 == a2) || (b1 == a2 && c1 == c2))
            {
                elAdj[i + 3] = b2;
                elAdj[j + 5] = a1;
            }
            // Edge 3 == Edge 1
            if ((c1 == a2 && a1 == b2) || (c1 == b2 && a1 == a2))
            {
                elAdj[i + 5] = c2;
                elAdj[j + 1] = b1;
            }
            // Edge 3 == Edge 2
            if ((c1 == b2 && a1 == c2) || (c1 == c2 && a1 == b2))
            {
                elAdj[i + 5] = a2;
                elAdj[j + 3] = b1;
            }
            // Edge 3 == Edge 3
            if ((c1 == c2 && a1 == a2) || (c1 == a2 && a1 == c2))
            {
                elAdj[i + 5] = b2;
                elAdj[j + 5] = b1;
            }
        }
    }

    // Look for any outside edges
    for (GLuint i = 0; i < elAdj.size(); i += 6)
    {
        if (elAdj[i + 1] == std::numeric_limits<GLuint>::max()) elAdj[i + 1] = elAdj[i + 4];
        if (elAdj[i + 3] == std::numeric_limits<GLuint>::max()) elAdj[i + 3] = elAdj[i];
        if (elAdj[i + 5] == std::numeric_limits<GLuint>::max()) elAdj[i + 5] = elAdj[i + 2];
    }

    // Copy all data back into el
    faces = elAdj;
}

void ObjMesh::SetName(std::string name) { m_name = name; }
std::string ObjMesh::GetName() { return m_name; }

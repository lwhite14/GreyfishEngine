
OUTFILE = App.out
RM = rm -f
B = Build/
SOURCES = Source/AppEntry.cpp Source/Drawables/Cube.cpp Source/Drawables/ObjMesh.cpp Source/imgui/imgui_demo.cpp Source/imgui/imgui_draw.cpp Source/imgui/imgui_impl_glfw.cpp Source/imgui/imgui_impl_opengl3.cpp Source/imgui/imgui_tables.cpp Source/imgui/imgui_widgets.cpp Source/imgui/imgui.cpp Source/SceneObjects/SceneObject.cpp Source/Scenes/BasicScene.cpp Source/stb/stb_image.cpp Source/UserInterface/MasterUI.cpp Source/GLSLProgram.cpp Source/Texture.cpp glad.c 


build:
	if [ ! -d "./$(B)" ];then mkdir $(B); fi

	g++ -std=c++20 -o $(B)$(OUTFILE) $(SOURCES) -ldl -lglfw

run:
	./$(B)$(OUTFILE)

clean:
	$(RM) $(B)$(OUTFILE)
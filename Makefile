ifeq ($(OS),Windows_NT)
	OUTFILE = App.exe
	OSFLAG = -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG = -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG = -D IA32
	endif
	RM = del 
	B = Build/
	BUILDDIR= Build
else
	OUTFILE = App.out
	OSFLAG += -D Linux
	RM = rm -f
	B = Build/
endif

build:
	if [ ! -d "./$(B)" ];then mkdir $(B); fi

	g++ -c Source/Drawables/Cube.cpp -o $(B)Cube.o
	g++ -c Source/Drawables/ObjMesh.cpp -o $(B)ObjMesh.o
	g++ -c Source/imgui/imgui_demo.cpp -o $(B)imgui_demo.o
	g++ -c Source/imgui/imgui_draw.cpp -o $(B)imgui_draw.o
	g++ -c Source/imgui/imgui_impl_glfw.cpp -o $(B)imgui_impl_glfw.o
	g++ -c Source/imgui/imgui_impl_opengl3.cpp -o $(B)imgui_impl_opengl3.o
	g++ -c Source/imgui/imgui_tables.cpp -o $(B)imgui_tables.o
	g++ -c Source/imgui/imgui_widgets.cpp -o $(B)imgui_widgets.o
	g++ -c Source/imgui/imgui.cpp -o $(B)imgui.o
	g++ -c Source/SceneObjects/SceneObject.cpp -o $(B)SceneObject.o
	g++ -c Source/Scenes/BasicScene.cpp -o $(B)BasicScene.o
	g++ -c Source/stb/stb_image.cpp -o $(B)stb_image.o
	g++ -c Source/UserInterface/MasterUI.cpp -o $(B)MasterUI.o
	g++ -c Source/AppEntry.cpp -o $(B)AppEntry.o
	g++ -c Source/GLSLProgram.cpp -o $(B)GLSLProgram.o
	g++ -c Source/Texture.cpp -o $(B)Texture.o
	g++ -c glad.c -o $(B)glad.o

	g++ -std=c++20 $(B)Cube.o $(B)ObjMesh.o $(B)imgui_demo.o $(B)imgui_draw.o $(B)imgui_impl_glfw.o $(B)imgui_impl_opengl3.o $(B)imgui_tables.o $(B)imgui_widgets.o $(B)imgui.o $(B)SceneObject.o $(B)BasicScene.o $(B)stb_image.o $(B)MasterUI.o $(B)AppEntry.o $(B)GLSLProgram.o $(B)Texture.o $(B)glad.o -o $(B)$(OUTFILE) -ldl -lglfw

run:
	./$(B)$(OUTFILE)

clean:
	$(RM) $(B)$(OUTFILE) $(B)Cube.o $(B)ObjMesh.o $(B)imgui_demo.o $(B)imgui_draw.o $(B)imgui_impl_glfw.o $(B)imgui_impl_opengl3.o $(B)imgui_tables.o $(B)imgui_widgets.o $(B)imgui.o $(B)SceneObject.o $(B)BasicScene.o $(B)stb_image.o $(B)MasterUI.o $(B)AppEntry.o $(B)GLSLProgram.o $(B)Texture.o $(B)glad.o


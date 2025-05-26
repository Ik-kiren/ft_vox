CXX = c++ -I${HOME}/.brew/opt/freetype/include/freetype2
NAME = vox
SRCS = srcs/Sun.cpp srcs/SkyBox.cpp srcs/Vector3i.cpp srcs/utils.cpp srcs/cave.cpp srcs/mapGP.cpp srcs/biome.cpp srcs/ChunkManager.cpp srcs/NewMesh.cpp srcs/Renderer.cpp srcs/Block.cpp srcs/Chunk.cpp srcs/ComputeShader.cpp srcs/BSphere.cpp srcs/Button.cpp srcs/Font.cpp srcs/MatrixStack.cpp srcs/main.cpp srcs/Shader.cpp srcs/Vector2.cpp srcs/Vector3.cpp srcs/stb.cpp srcs/Vector4.cpp srcs/Matrix4.cpp srcs/Mesh.cpp srcs/ScopMaths.cpp srcs/Camera.cpp srcs/Object.cpp srcs/Player.cpp
CPPFLAGS = -Werror
LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL -lfreetype
OBJS = $(SRCS:cpp=o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(OBJS) $(LIBS) $(CPPFLAGS) -o $(NAME)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

dll:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install freetype
	git clone https://github.com/nothings/stb.git
	mv ./stb/stb_image.h ./includes
	rm  -rf ./stb

re: fclean all

home :
	g++ main.cpp Shader.cpp Vector3.cpp stb.cpp Vector4.cpp Matrix4.cpp Mesh.cpp ScopMaths.cpp Camera.cpp Object.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lGLU -lGL

.PHONY: all clean fclean re home
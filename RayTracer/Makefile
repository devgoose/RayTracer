SRC_DIR := ./
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
LDFLAGS := 
CPPFLAGS := -lm
CXXFLAGS := 

render: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	g++ -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf obj

all:
	g++ -o render render_main.cpp Color.cpp DepthCue.cpp DirectionalLight.cpp Light.cpp Material.cpp Point3.cpp PointLight.cpp Ray.cpp Scene.cpp SceneObject.cpp Sphere.cpp Vector3.cpp ViewWindow.cpp

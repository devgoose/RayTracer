SRC_DIR := ./
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
LDFLAGS := 
CPPFLAGS := 
CXXFLAGS := -MMD
-include $(OBJ_FILES:.o=.d)

render: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

all:
	g++ -o render render_main.cpp Color.cpp DepthCue.cpp DirectionalLight.cpp Light.cpp Material.cpp Point3.cpp PointLight.cpp Ray.cpp Scene.cpp SceneObject.cpp Sphere.cpp Vector3.cpp ViewWindow.cpp
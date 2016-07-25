#include "Voxels.h"

Model Voxels::loadModel(const std::string& objPath, const std::string& palettePath)
{
	loadPixIntoVec(palettePath);
    Model m = loadObjData(objPath);

	return m;
}

Model Voxels::loadObjData(const std::string& objPath)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<std::string> faces;

    std::ifstream file(objPath);
    std::string line;
    while (getline(file, line))
    {
        std::vector<std::string> splitString = splitStr(line, ' ');

        if (splitString[0] == "v") //vertex
            vertices.push_back(glm::vec3(std::stof(splitString[1]), std::stof(splitString[2]), std::stof(splitString[3])));
        if (splitString[0] == "vt")//texture
            texCoords.push_back(glm::vec2(std::stof(splitString[1]), std::stof(splitString[2])));
        if (splitString[0] == "vn")//normal
            normals.push_back(glm::vec3(std::stof(splitString[1]), std::stof(splitString[2]), std::stof(splitString[3])));
        if (splitString[0] == "f")
            faces.push_back(line);
    }
    file.close();

    int count = 0;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::vector<std::string> faceLine = splitStr(faces[i], ' ');

        for (unsigned int ii = 1; ii <= 3; ii++)
        {
            std::vector<std::string> indices = splitStr(faceLine[ii], '/');
            const int vertIndex = std::stoi(indices[0]) - 1;
            const int texIndex = std::stoi(indices[1]) - 1;
            const int normIndex = std::stoi(indices[2]) - 1;
            glm::vec3 vertex = vertices[vertIndex];
            glm::vec2 texCoord = texCoords[texIndex];
            glm::vec3 normal = normals[normIndex];
            data.push_back(vertex.x); data.push_back(vertex.y); data.push_back(vertex.z);
            data.push_back(texCoord.x); data.push_back(texCoord.y);
            data.push_back(normal.x); data.push_back(normal.y); data.push_back(normal.z);
            count++;
        }
    }

    Model _model = Model{ currentIndex, count };
    currentIndex += count;

	return _model;
}


std::vector<std::string> Voxels::splitStr(const std::string & toSplit, char splitter) 
{
    std::vector<std::string> splitString;
    std::ostringstream current;

    for (unsigned int i = 0; i < toSplit.length(); i++)
    {
        char currentChar;
        if ((currentChar = toSplit.at(i)) != splitter)
            current << currentChar;
        else
        {
            std::string stringSect = current.str();
            splitString.push_back(stringSect);
            current.str("");
            current.clear();
        }
    }

    std::string stringSect = current.str();
    splitString.push_back(stringSect);

    return splitString;
}

int Voxels::loadPixIntoVec(const std::string& palettePath)
{
	std::vector<GLubyte> lodeData;//data from lodepng
	unsigned int width, height;
	unsigned int err = lodepng::decode(lodeData, width, height, palettePath);
	if (err) {
		std::string err_text = lodepng_error_text(err);
		std::cerr << "Error loading image: " << palettePath << ": " << err_text << endl;
		system("pause");  exit(1);
	}

	if (height != 1)
		std::cerr << "Palette: " << palettePath << " is not one pixel tall!!" << endl;
	if (width != 256)
		std::cerr << "Palettes are supposed to be 256pix wide from MagicaVoxel, if this is wrong, contact Ned" << endl;

	for (unsigned int x = 0; x < lodeData.size(); x+=4)
	{
		GLubyte r = lodeData[x];
		GLubyte g = lodeData[x + 1];
		GLubyte b = lodeData[x + 2];
		GLubyte a = lodeData[x + 3];
		GLuint fullColor = r << 24 | g << 16 | b << 8 | a;
		pixels.push_back(fullColor);
	}

	return currentY;
}

void Voxels::setDrawingStage()
{
	loadingStage = false;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glEnableVertexAttribArray(VERTEX_ATTRIB);
    glEnableVertexAttribArray(TEXTURE_ATTRIB);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(0));
    glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
    glBindVertexArray(0);

}

void Voxels::destroy()
{
    glDeleteVertexArrays(1, &vao);//releases vbo, to also be deleted
    glDeleteBuffers(1, &vbo);
}

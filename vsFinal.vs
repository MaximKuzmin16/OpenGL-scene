#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;   // Передаем позицию фрагмента во фрагментный шейдер
out vec4 MVMat;
out vec3 Normal;    // Передаем нормаль во фрагментный шейдер

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));  // Преобразуем позицию в мировые координаты
	MVMat = view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Преобразуем нормали в мировые координаты
}
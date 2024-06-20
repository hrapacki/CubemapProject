#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    if (Normal.z != 0.0) {
        R.z = -R.z;
    }

    FragColor = texture(skybox, R);
}

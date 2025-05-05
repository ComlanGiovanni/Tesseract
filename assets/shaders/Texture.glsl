#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

// out vec4 FragColor; // Remplacé par gl_FragColor pour tester

in vec4 v_Color;

// uniform sampler2D u_Texture; // On l'utilisera plus tard

void main()
{
    // FragColor = v_Color;
    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0); // Couleur fixe magenta pour test
}

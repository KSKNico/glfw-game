static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"layout (location = 1) in vec3 vCol;\n"
"layout (location = 0) in vec3 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
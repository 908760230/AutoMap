#include "Render.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>

const char* landFrag =
"uniform sampler2D u_water;\n"
"uniform float u_outline_water;\n"
"uniform float v_e;\n"
"uniform vec2 v_xy;\n"
"void main() {\n"
"	float e = 0.5 * (1.0 + v_e)\n"
"	float river = texture2D(u_water, v_xy).a\n"
"	if (e >= 0.5) {\n"
"		float bump = u_outline_water / 256.0\n"
"		float L1 = e + bump\n"
"		float L2 = (e - 0.5) * (bump * 100.0) + 0.5\n"
// TODO: simplify equation
"		e = min(L1, mix(L1, L2, river))\n"
"	}\n"
"	gl_FragColor = vec4(fract(256.0 * e), e, 0, 1)\n"
"}\n";

const char* landVert =
"uniform mat4 u_projection;\n"
"attribute vec2 a_xy;\n"
"attribute vec2 a_em;\n" // NOTE: moisture channel unused
"uniform float v_e;\n"
"uniform vec2 v_xy;\n"
"void main() {\n"
"    vec4 pos = vec4(u_projection * vec4(a_xy, 0, 1))\n"
"    v_xy = (1.0 + pos.xy) * 0.5\n"
"    v_e = a_em.x\n"
"    gl_Position = pos\n"
"}\n";

const char* riverFrag =
"uniform sampler2D u_rivertexturemap;\n"
"uniform vec2 v_uv;\n"
"vec3 blue = vec3(0.2, 0.5, 0.7);\n"
"void main() {\n"
"	vec4 color = texture2D(u_rivertexturemap, v_uv)\n"
"	gl_FragColor = vec4(blue * color.a, color.a)\n"
"}\n";

const char* riverVert = 
"uniform mat4 u_projection;\n"
"uniform vec4 a_xyuv;\n"
"uniform vec2 v_uv;\n"
"void main() {\n"
"	v_uv = a_xyuv.ba\n"
"	gl_Position = vec4(u_projection * vec4(a_xyuv.xy, 0, 1))\n"
"}\n";


const char* depthFrag =
"uniform float v_z;\n"
"void main() {\n"
"	gl_FragColor = vec4(fract(256.0 * v_z), floor(256.0 * v_z) / 256.0, 0, 1)\n"
"}\n";

const char* depthVert =
"uniform mat4 u_projection;\n"
"uniform vec2 a_xy;\n"
"uniform vec2 a_em;\n"
"uniform float v_z;\n"
"void main() {\n"
"   vec4 pos = vec4(u_projection * vec4(a_xy, max(0.0, a_em.x), 1))\n"
"   v_z = a_em.x\n"
"   gl_Position = pos\n"
"}\n";

const char* drapeFrag =
"uniform sampler2D u_colormap;\n"
"uniform sampler2D u_mapdata;\n"
"uniform sampler2D u_water;\n"
"uniform sampler2D u_depth;\n"
"uniform vec2 u_light_angle;\n"
"uniform float u_inverse_texture_size,\n"
"u_slope, u_flat,\n"
"u_ambient, u_overhead,\n"
"u_outline_strength, u_outline_coast, u_outline_water,\n"
"u_outline_depth, u_outline_threshold,\n"
"u_biome_colors;\n"
"uniform vec2 v_uv, v_xy, v_em;\n"
"uniform float v_z;\n"
"vec2 _decipher = vec2(1.0 / 256.0, 1);\n"
"float decipher(vec4 v) {\n"
"    return dot(_decipher, v.xy);\n"
"}\n"
"vec3 neutral_land_biome = vec3(0.9, 0.8, 0.7);\n"
"vec3 neutral_water_biome = 0.8 * neutral_land_biome;\n"
"void main() {\n"
"    vec2 sample_offset = vec2(0.5 * u_inverse_texture_size, 0.5 * u_inverse_texture_size);\n"
"    vec2 pos = v_uv + sample_offset;\n"
"    vec2 dx = vec2(u_inverse_texture_size, 0),\n"
"        dy = vec2(0, u_inverse_texture_size);\n"
"    float zE = decipher(texture2D(u_mapdata, pos + dx));\n"
"    float zN = decipher(texture2D(u_mapdata, pos - dy));\n"
"    float zW = decipher(texture2D(u_mapdata, pos - dx));\n"
"    float zS = decipher(texture2D(u_mapdata, pos + dy));\n"
"    vec3 slope_vector = normalize(vec3(zS - zN, zE - zW, u_overhead * 2.0 * u_inverse_texture_size));\n"
"    vec3 light_vector = normalize(vec3(u_light_angle, mix(u_slope, u_flat, slope_vector.z)));\n"
"    float light = u_ambient + max(0.0, dot(light_vector, slope_vector));\n"
"    vec2 em = texture2D(u_mapdata, pos).yz;\n"
"   em.y = v_em.y;\n"
"    vec3 neutral_biome_color = neutral_land_biome;\n"
"    vec4 water_color = texture2D(u_water, pos);\n"
"    if (em.x >= 0.5 && v_z >= 0.0) {\n"
"        // on land, lower the elevation around rivers\n"
"        em.x -= u_outline_water / 256.0 * (1.0 - water_color.a);\n"
"    }\n"
"    else {\n"
"        // in the ocean, or underground, don't draw rivers\n"
"        water_color.a = 0.0; neutral_biome_color = neutral_water_biome;\n"
"    }\n"
"    vec3 biome_color = texture2D(u_colormap, em).rgb;\n"
"    water_color = mix(vec4(neutral_water_biome * (1.2 - water_color.a), water_color.a), water_color, u_biome_colors);\n"
"    biome_color = mix(neutral_biome_color, biome_color, u_biome_colors);\n"
"    if (v_z < 0.0) {\n"
"        // at the exterior boundary, we'll draw soil or water underground\n"
"        float land_or_water = smoothstep(0.0, -0.001, v_em.x - v_z);\n"
"        vec3 soil_color = vec3(0.4, 0.3, 0.2);\n"
"        vec3 underground_color = mix(soil_color, mix(neutral_water_biome, vec3(0.1, 0.1, 0.2), u_biome_colors), land_or_water) * smoothstep(-0.7, -0.1, v_z);\n"
"        vec3 highlight_color = mix(vec3(0, 0, 0), mix(vec3(0.8, 0.8, 0.8), vec3(0.4, 0.5, 0.7), u_biome_colors), land_or_water);\n"
"        biome_color = mix(underground_color, highlight_color, 0.5 * smoothstep(-0.025, 0.0, v_z));\n"
"        light = 1.0 - 0.3 * smoothstep(0.8, 1.0, fract((v_em.x - v_z) * 20.0)); // add horizontal lines\n"
"    }\n"
"    // if (fract(em.x * 10.0) < 10.0 * fwidth(em.x)) { biome_color = vec3(0,0,0); } // contour lines\n"
"    // TODO: add noise texture based on biome\n"

"    // TODO: once I remove the elevation rounding artifact I can simplify\n"
"    // this by taking the max first and then deciphering\n"
"    float depth0 = decipher(texture2D(u_depth, v_xy)),\n"
"        depth1 = max(max(decipher(texture2D(u_depth, v_xy + u_outline_depth * (-dy - dx))),\n"
"            decipher(texture2D(u_depth, v_xy + u_outline_depth * (-dy + dx)))),\n"
"            decipher(texture2D(u_depth, v_xy + u_outline_depth * (-dy)))),\n"
"        depth2 = max(max(decipher(texture2D(u_depth, v_xy + u_outline_depth * (dy - dx))),\n"
"            decipher(texture2D(u_depth, v_xy + u_outline_depth * (dy + dx)))),\n"
"            decipher(texture2D(u_depth, v_xy + u_outline_depth * (dy))));\n"
"    float outline = 1.0 + u_outline_strength * (max(u_outline_threshold, depth1 - depth0) - u_outline_threshold);\n"
"    // Add coast outline, but avoid it if there's a river nearby\n"
"    float neighboring_river = max(\n"
"        max(\n"
"            texture2D(u_water, pos + u_outline_depth * dx).a,\n"
"            texture2D(u_water, pos - u_outline_depth * dx).a\n"
"        ),\n"
"        max(\n"
"            texture2D(u_water, pos + u_outline_depth * dy).a,\n"
"            texture2D(u_water, pos - u_outline_depth * dy).a\n"
"        )\n"
"    );\n"
"    if (em.x <= 0.5 && max(depth1, depth2) > 1.0 / 256.0 && neighboring_river <= 0.2) { outline += u_outline_coast * 256.0 * (max(depth1, depth2) - 2.0 * (em.x - 0.5)); }\n"
"   gl_FragColor = vec4(mix(biome_color, water_color.rgb, water_color.a) * light / outline, 1);\n"
"}\n";

const char* drapeVert =
"uniform mat4 u_projection;\n"
"attribute vec2 a_xy;\n"
"attribute vec2 a_em;\n"
"varying vec2 v_em, v_uv, v_xy;\n"
"varying float v_z;\n"
"void main() {\n"
"    v_em = a_em;\n"
"    vec2 xy_clamped = clamp(a_xy, vec2(0, 0), vec2(1000, 1000));\n"
"    v_z = max(0.0, a_em.x); // oceans with e<0 still rendered at z=0\n"
"    if (xy_clamped != a_xy) { // boundary points\n"
"        v_z = -0.5;\n"
"        v_em = vec2(0.0, 0.0);\n"
"    }\n"
"    vec4 pos = vec4(u_projection * vec4(xy_clamped, v_z, 1));\n"
"    v_uv = a_xy / 1000.0;\n"
"    v_xy = (1.0 + pos.xy) * 0.5;\n"
"    gl_Position = pos;\n"
"}\n";

const char* finalFrag =
"uniform sampler2D u_texture;\n"
"uniform vec2 u_offset;\n"
"varying vec2 v_uv;\n"
"void main() {\n"
"	gl_FragColor = texture2D(u_texture, v_uv + u_offset);\n"
"}\n";

const char* finalVert =
"uniform vec2 a_uv;\n"
"varying vec2 v_uv;\n"
"void main() {\n"
"	v_uv = a_uv;\n"
"	gl_Position = vec4(2.0 * v_uv - 1.0, 0.0, 1.0);\n"
"}\n";

Render::Render(const TriangleMesh& mesh)
{
	m_TopDown = glm::translate(glm::mat4(1.0f), glm::vec3(1, -1, 0));
	m_TopDown = glm::scale(m_TopDown, glm::vec3(1 / 500.0, 1 / 500.0, 1));
	a_quad_xy.resize(2 * (mesh.numRegions + mesh.numTriangles));
	a_quad_em.resize(2 * (mesh.numRegions + mesh.numTriangles));
	quad_elements.resize(3 * mesh.numSolidSides);
	a_river_xyuv.resize(1.5 * 3 * 4 * mesh.numSolidTriangles);
	setMeshGeometry(mesh);
}

Render::~Render()
{
}

glm::vec2 Render::screenToWorld(double x, double y)
{
	auto result = glm::vec4(x * 2 - 1, 1 - y * 2, 0, 1) * m_inverse_projection;
	return glm::vec2(result.x,result.y);
}

void Render::setMeshGeometry(const TriangleMesh& mesh)
{
	if (a_quad_xy.size() !=  2 * (mesh.numRegions + mesh.numTriangles)) { 
		throw std::runtime_error("wrong size"); 
	}
	int p = 0;
	for (int r = 0; r < mesh.numRegions; r++) {
		a_quad_xy[p++] = mesh.x_of_r(r);
		a_quad_xy[p++] = mesh.y_of_r(r);
	}
	for (int t = 0; t < mesh.numTriangles; t++) {
		a_quad_xy[p++] = mesh.x_of_t(t);
		a_quad_xy[p++] = mesh.y_of_t(t);
	}
}

void Render::drawLand()
{
}

void Render::drawRiver()
{
}

void Render::drawDrape()
{
}

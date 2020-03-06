# glm  
GLM is a header-only library,and thus does not need to be compiled.  

Include all GLM core / GLSL features
- #include glm/glm.hpp // vec2, vec3, mat4, radians  

Include all GLM extensions 为稳定版本的扩展 
- #include glm/ext.hpp // perspective, translate, rotate  

GLM extends the core GLSL feature set with extensions. These extensions include: quaternion, transformation,
spline, matrix inverse, color spaces, etc.  
- include glm/gtc/*    推荐使用的扩展

## Preprocessor configurations  
- GLM_FORCE_MESSAGES: Platform auto detection and default configuration  
- GLM_FORCE_PLATFORM_UNKNOWN: Force GLM to no detect the build platform  
- GLM_FORCE_COMPILER_UNKNOWN: Force GLM to no detect the C++ compiler 
- GLM_FORCE_ARCH_UNKNOWN: Force GLM to no detect the build architecture  
- GLM_FORCE_CXX**: C++ language detection  



lookAt:
  GLM_FUNC_QUALIFIER mat<4, 4, T, Q> lookAtRH(vec<3, T, Q> const& eye, vec<3, T, Q> const& center, vec<3, T, Q> const& up)
	{
		vec<3, T, Q> const f(normalize(center - eye));
		vec<3, T, Q> const s(normalize(cross(f, up)));
		vec<3, T, Q> const u(cross(s, f));

		mat<4, 4, T, Q> Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] =-f.x;
		Result[1][2] =-f.y;
		Result[2][2] =-f.z;
		Result[3][0] =-dot(s, eye);
		Result[3][1] =-dot(u, eye);
		Result[3][2] = dot(f, eye);
		return Result;
	}

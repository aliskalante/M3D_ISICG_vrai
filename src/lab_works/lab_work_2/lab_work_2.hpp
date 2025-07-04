#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		void			   Optional( Vec2f C, int N, float R );
		std::vector<Vec3f> color_sommets;
		std::vector<int>   indexessommets;
		std::vector<Vec2f> Positionsommets;
		const std::string  fragmentShaderStr;
		const std::string  vertexShaderStr;

		// ================ GL data.
		GLuint vertexShader, fragmentShader;
		GLuint Program;
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;
		GLuint VBO_color;
		GLint  loc_translation;
		GLint  loc_luminosité;
		double _time	  = 0;
		float  Luminosité = 1;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
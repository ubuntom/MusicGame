#include <string>

class InvalidScore: public exception
{
	public:
		HitScore hs;
		InvalidScore(HitScore _hs)
		{
			hs = _hs;
		}
		virtual const char* what() const throw()
		{
			return "A non-numeric hitscore was added to the score.";
		}
};


class FileOpenError: public exception
{
	
	public:
		std::string filename;
		FileOpenError(const char* fname):
			filename(fname)
		{}
		virtual const char* what() const throw()
		{
			return "A file could not be opened";
		}
};

class BeatmapOpenError: public FileOpenError
{
	public:
		BeatmapOpenError(const char* fname):
			FileOpenError(fname)
		{}
		virtual const char* what() const throw()
		{
			return "A beatmap could not be opened";
		}
};


class SliderFramebufferError: public exception
{
	public:
		GLenum errorcode;
		SliderFramebufferError(GLenum code):
			errorcode(code)
		{}
		
		virtual const char* what() const throw()
		{
			switch(errorcode)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"; break;
				case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
				case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
				default:											return "Unknown error. Query errorcode"; break;
			}
		}
			
};

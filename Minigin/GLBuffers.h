// The warning its ignoring is that the two static functions aren`t being used, but it is used in MeshComponent, so thats why it is disabled
#pragma warning(disable : 4505)

#include <string>
#include <vector>
#include <glad/glad.h>
#include <stdexcept>




// this design is credit: https://www.youtube.com/watch?v=rkxrw8dNrvI&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=44
// For now Im following this tutorial, until I understand how it works and will probably refactor  a lot

namespace Jotar
{
	enum class ShaderDataType
	{
		None, Float, Vec2, Vec3, Vec4, Mat3, Mat4, Int, Int2, Int3, Int4, BOOL
	};

	static unsigned int ShaderDataTypeSize(Jotar::ShaderDataType type)
	{
		switch (type)
		{
		case Jotar::ShaderDataType::Float:
		case Jotar::ShaderDataType::Int:
			return 4;
		case Jotar::ShaderDataType::Vec2:
		case Jotar::ShaderDataType::Int2:
			return 4 * 2;
		case Jotar::ShaderDataType::Vec3:
		case Jotar::ShaderDataType::Int3:
			return 4 * 3;
		case Jotar::ShaderDataType::Vec4:
		case Jotar::ShaderDataType::Int4:
			return 4 * 4;
		case Jotar::ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case Jotar::ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case Jotar::ShaderDataType::BOOL:
			return 1;
		default:
			throw std::runtime_error(std::string("Unknown ShaderDataType"));
		}
	}

	static GLenum ShaderDataTypeToOpenGLBaseType(Jotar::ShaderDataType type)
	{
		switch (type)
		{
		case Jotar::ShaderDataType::Float:
		case Jotar::ShaderDataType::Vec2:
		case Jotar::ShaderDataType::Vec3:
		case Jotar::ShaderDataType::Vec4:
		case Jotar::ShaderDataType::Mat3:
		case Jotar::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Jotar::ShaderDataType::Int:
		case Jotar::ShaderDataType::Int2:
		case Jotar::ShaderDataType::Int3:
		case Jotar::ShaderDataType::Int4:
			return GL_INT;
		case Jotar::ShaderDataType::BOOL:
			return GL_BOOL;
		default:
			throw std::runtime_error(std::string("Unknown ShaderDataType"));
		}
	}



	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned int Offset;
		unsigned int Size;
		bool Normalized;

		BufferElement()
			: Name{ "" }, Type{ ShaderDataType::None }, Size{ 0 }, Offset{ 0 }, Normalized{ false }
		{}

		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name{ name }, Type{ type }, Size{ ShaderDataTypeSize(type)}, Offset{0}, Normalized{normalized}
		{
		}

		unsigned int GetElementComponentCount() const
		{
			switch (Type)
			{
			case Jotar::ShaderDataType::Float:
			case Jotar::ShaderDataType::Int:
			case Jotar::ShaderDataType::BOOL:
				return 1;
			case Jotar::ShaderDataType::Vec2:
			case Jotar::ShaderDataType::Int2:
				return 2;
			case Jotar::ShaderDataType::Vec3:
			case Jotar::ShaderDataType::Int3:
				return 3;
			case Jotar::ShaderDataType::Int4:
			case Jotar::ShaderDataType::Vec4:
				return 4;
			case Jotar::ShaderDataType::Mat3:
				return 3 * 3;
			case Jotar::ShaderDataType::Mat4:
				return 4 * 4;

			default:
				throw std::runtime_error(std::string("Unknown ShaderDataType"));
			}
		}
	};


	class BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements{ elements }
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }

	private:

		void CalculateOffsetAndStride()
		{
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;

			}
		}


		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};



	class GLVertexBuffer
	{
	public:
		GLVertexBuffer(float* vertices, size_t size);
		~GLVertexBuffer();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_VertexBufferID;

	};

	class GLIndexBuffer
	{
	public:
		GLIndexBuffer(unsigned int* vertices, unsigned int count);
		~GLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const;


	private:
		unsigned int m_IndexBufferID;
		unsigned int m_Count;
	};
}
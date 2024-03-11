#pragma once

#include "Component.h"
#include "GameObject.h"
#include "ShaderComponent.h"
#include <stdexcept>


//REFACTOR
#include "glad/glad.h"

namespace dae
{



	/// <This part is made based from this video, All of this will be refactored, this is more for understanding this process and how this works for now>
	/// 
	/// /// https://www.youtube.com/watch?v=jIJFM_pi6gQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=31
	/// </summary>
	/// 
	/// 
	/*enum class ShaderDataType
	{
		None, Float, Vec2, Vec3, Vec4, Mat3, Mat4, Int, Int2, Int3, Int4, BOOL
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case dae::ShaderDataType::Float:
		case dae::ShaderDataType::Int:
			return 4;
		case dae::ShaderDataType::Vec2:
		case dae::ShaderDataType::Int2:
			return 4 * 2;
		case dae::ShaderDataType::Vec3:
		case dae::ShaderDataType::Int3:
			return 4 * 3;
		case dae::ShaderDataType::Vec4:
		case dae::ShaderDataType::Int4:
			return 4 * 4;
		case dae::ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case dae::ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case dae::ShaderDataType::BOOL:
			return 1;
		default:
			throw std::runtime_error(std::string("Unknown ShaderDataType"));
			return 0;
		}
	}

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case dae::ShaderDataType::Float:
		case dae::ShaderDataType::Vec2:
		case dae::ShaderDataType::Vec3:
		case dae::ShaderDataType::Vec4:
		case dae::ShaderDataType::Mat3:
		case dae::ShaderDataType::Mat4:
			return GL_FLOAT;
		case dae::ShaderDataType::Int:
		case dae::ShaderDataType::Int2:
		case dae::ShaderDataType::Int3:
		case dae::ShaderDataType::Int4:
			return GL_INT;
		case dae::ShaderDataType::BOOL:
			return GL_BOOL;
		default:
			throw std::runtime_error(std::string("Unknown ShaderDataType"));
			return 0;
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
			: Name{ name }, Type{ type }, Size{ 0 }, Offset{ 0 }, Normalized{ normalized }
		{
		}

		unsigned int GetElementComponentCount() const
		{
			switch (Type)
			{
			case dae::ShaderDataType::Float:
			case dae::ShaderDataType::Int:
			case dae::ShaderDataType::BOOL:
				return 1;
			case dae::ShaderDataType::Vec2:
			case dae::ShaderDataType::Int2:
				return 2;
			case dae::ShaderDataType::Vec3:
			case dae::ShaderDataType::Int3:
				return 3;
			case dae::ShaderDataType::Int4:
			case dae::ShaderDataType::Vec4:
				return 4;
			case dae::ShaderDataType::Mat3:
				return 3 * 3;
			case dae::ShaderDataType::Mat4:
				return 4 * 4;

			default:
				throw std::runtime_error(std::string("Unknown ShaderDataType"));
				return 0;
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
	};*/




	/// <summary>
	/// /
	/// 
	/// 
	/// </summary>



	class Mesh2DComponent : public Component
	{
	public: 
		Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions);
		Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions, unsigned int* elementArray, unsigned int elementCount);
		~Mesh2DComponent();

		virtual void Update();

		virtual void Init();
		virtual void Render() const;

		//void SetLayout(const BufferLayout& layout);
		//const BufferLayout& GetLayout() const;

		void Bind() const;
		void Unbind() const;

	private:


		//BufferLayout m_Layout;
		unsigned int m_VertexCount, m_ElementCount;
		unsigned int m_VertexArray, m_ElementBuffer;
		unsigned int m_VertexBuffer;
		ShaderComponent* m_ShaderComponent;
	};
}
#pragma once

class Buffer
{
public:
	enum class Type
	{
		Unknown,
		Uniform,
		Vertex,
		Index,
		FrameBuffer
	};
	Buffer() {}
	Buffer::Type GetType() { return Type::Unknown; };
	virtual~Buffer() {}
};


#include "FilterProgram.h"
#include "../parser/PositionTrans.h"
#include "../parser/AttributeNode.h"
#include "../parser/VaryingNode.h"
#include "../parser/TextureMap.h"
#include "../parser/FragColor.h"

namespace sl
{

FilterProgram::FilterProgram(RenderContext* rc, int max_vertex)
	: ShaderProgram(rc, max_vertex)
{
}

void FilterProgram::Init(const std::vector<VertexAttrib>& va_list, 
						 RenderBuffer* ib, parser::Node* pn)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		pn)->Connect(
		new parser::FragColor());

	Load(vert, frag, va_list, ib, true);
}

}
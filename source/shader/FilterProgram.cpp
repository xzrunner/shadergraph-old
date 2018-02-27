#include "shaderlab/FilterProgram.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/AttributeNode.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/TextureMap.h"
#include "shaderlab/FragColor.h"

namespace sl
{

FilterProgram::FilterProgram(RenderContext& rc, int max_vertex)
	: ShaderProgram(rc, max_vertex)
{
}

void FilterProgram::Init(const CU_VEC<ur::VertexAttrib>& va_list, 
						 const std::shared_ptr<RenderBuffer>& ib, parser::Node* pn, parser::Node* pre_pn/* = nullptr*/)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")));

	parser::Node* frag = new parser::TextureMap();
	if (pre_pn) {
		frag->Connect(
			pre_pn)->Connect(
			pn)->Connect(
			new parser::FragColor());
	} else {
		frag->Connect(
			pn)->Connect(
			new parser::FragColor());
	}

	Load(vert, frag, va_list, ib, true);
}

}
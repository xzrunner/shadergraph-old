#ifndef _SHADERLAB_PARSER_VARYING_PASS_H_
#define _SHADERLAB_PARSER_VARYING_PASS_H_

#include "Node.h"
#include "VariableType.h"

namespace sl
{
namespace parser
{

class VaryingPass : public Node
{
public:
	VaryingPass(VariableType type, std::string name);

	virtual std::string& ToStatements(std::string& str) const;

private:
	std::string m_name;

}; // VaryingPass

}
}

#endif // _SHADERLAB_PARSER_VARYING_PASS_H_
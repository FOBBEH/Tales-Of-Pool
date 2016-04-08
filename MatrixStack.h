// [MatrixStack.h]
// there is no spoone - teh matricks
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <d3dx9.h>
#include <stack>
#include <assert.h>

struct MatrixStack
{
	MatrixStack();
	
	void Push( D3DXMATRIXA16 &matrix );
	void Pop();
	
	std::stack< D3DXMATRIXA16 > m_stack;
};

extern MatrixStack g_matrixStack;

#endif // MATRIXSTACK_H
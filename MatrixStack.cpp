#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
	D3DXMATRIXA16 identity;
	D3DXMatrixIdentity( &identity );
	m_stack.push( identity );
}

void MatrixStack::Push( D3DXMATRIXA16 &matrix )
{
	D3DXMATRIXA16 multiplied;

	D3DXMatrixMultiply( &multiplied, &matrix, &m_stack.top() );
	m_stack.push( multiplied );
}

void MatrixStack::Pop()
{
	m_stack.pop();

	assert( m_stack.size() != 0 );
}
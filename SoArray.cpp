//----------------------------------------------------------------
#include "SoArray.h"
#include <string> //Ϊ���ܹ����� calloc free memcpy �Ⱥ���
//----------------------------------------------------------------
SoArray::SoArray()
:m_pBuff(0)
,m_nMaxCount(0)
,m_nCurrentCount(0)
,m_nElementSize(0)
,m_nAddCount(0)
{

}
//----------------------------------------------------------------
SoArray::~SoArray()
{
	if (m_pBuff)
	{
		free(m_pBuff);
		m_pBuff = 0;
	}
}
//----------------------------------------------------------------
bool SoArray::InitArray(int nElementSize, int nInitCount, int nAddCount)
{
	m_nCurrentCount = 0;
	m_nElementSize = nElementSize;
	m_nAddCount = nAddCount;
	return Resize(nInitCount);
}
//----------------------------------------------------------------
int SoArray::PushBack(const void* pElement)
{
	if (pElement == 0)
	{
		return -1;
	}
	if (m_pBuff == 0)
	{
		return -1;
	}
	if (m_nCurrentCount >= m_nMaxCount)
	{
		if (Resize(m_nMaxCount + m_nAddCount) == false)
		{
			return -1;
		}
	}
	memcpy(m_pBuff+m_nCurrentCount*m_nElementSize, pElement, m_nElementSize);
	++m_nCurrentCount;
	return (m_nCurrentCount-1);
}
//----------------------------------------------------------------
bool SoArray::InsertAt(int nIndex, const void* pElement)
{
	if (nIndex < 0 || nIndex >= m_nMaxCount)
	{
		return false;
	}
	if (pElement == 0)
	{
		return false;
	}
	if (m_pBuff == 0)
	{
		return false;
	}
	memcpy(m_pBuff+nIndex*m_nElementSize, pElement, m_nElementSize);
	return true;
}
//----------------------------------------------------------------
bool SoArray::SwapElement(int nIndexA, int nIndexB)
{
	if (nIndexA == nIndexB)
	{
		return true;
	}
	if (nIndexA < 0 || nIndexA >= m_nMaxCount)
	{
		return false;
	}
	if (nIndexB < 0 || nIndexB >= m_nMaxCount)
	{
		return false;
	}
	if (m_pBuff == 0)
	{
		return false;
	}
	char* tempBuff = (char*)calloc(1, m_nElementSize);
	if (tempBuff == 0)
	{
		return false;
	}
	char* pA = m_pBuff + nIndexA * m_nElementSize;
	char* pB = m_pBuff + nIndexB * m_nElementSize;
	memcpy(tempBuff, pA, m_nElementSize);
	memcpy(pA, pB, m_nElementSize);
	memcpy(pB, tempBuff, m_nElementSize);
	free(tempBuff);
	return true;
}
//----------------------------------------------------------------
bool SoArray::Resize(int nMaxCount)
{
	if (nMaxCount <= m_nMaxCount)
	{
		nMaxCount = m_nMaxCount * 2;
	}
	//�����µ��ڴ�
	const int newSize = m_nElementSize * nMaxCount;
	char* newBuff = (char*)calloc(1, newSize);
	if (newBuff == 0)
	{
		return false;
	}
	//�Ѿ������Ԫ��������䵽�������У����ͷž�����
	if (m_pBuff)
	{
		if (m_nCurrentCount > 0)
		{
			memcpy(newBuff, m_pBuff, m_nElementSize*m_nCurrentCount);
		}
		free(m_pBuff);
		m_pBuff = 0;
	}
	//��������ά������
	m_pBuff = newBuff;
	m_nMaxCount = nMaxCount;
	return true;
}
//----------------------------------------------------------------

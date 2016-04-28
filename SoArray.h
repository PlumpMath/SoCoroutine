//----------------------------------------------------------------
#ifndef _SoArray_h_
#define _SoArray_h_
//----------------------------------------------------------------
class SoArray
{
public:
	SoArray();
	~SoArray();
	//��ʼ��������Ҳ������Ϊ�������ú�������һ�����������ó������顣
	//--nElementSize Ԫ�ش�С������Ԫ��ִ��sizeof����õ��ķ���ֵ��
	//--nInitCount ��ʼʱ�����Ԫ�ظ�����Ҳ��Ԥ������ڴ��ܹ����ɶ��ٸ�Ԫ�ء�
	//--nAddCount ������Ԫ��������Ԥ��������ڴ�ʱ�������ӵ��ڴ��ܹ����ɶ��ٸ�Ԫ�ء�
	bool InitArray(int nElementSize, int nInitCount, int nAddCount);
	//����ֵΪ��Ԫ�ص�λ����š�
	int PushBack(const void* pElement);
	bool InsertAt(int nIndex, const void* pElement);
	bool SwapElement(int nIndexA, int nIndexB);
	void* GetElement(int nIndex) const;
	//ɾ�����е�Ԫ�ء�m_pBuff�������·��䡣
	void Clear();
	int GetCount() const;

private:
	bool Resize(int nMaxCount);

private:
	char* m_pBuff;
	int m_nMaxCount;
	int m_nCurrentCount;
	int m_nElementSize;
	int m_nAddCount;
};
//----------------------------------------------------------------
inline void* SoArray::GetElement(int nIndex) const
{
	if (m_pBuff && nIndex >= 0 && nIndex < m_nMaxCount)
	{
		return (m_pBuff + nIndex * m_nElementSize);
	}
	else
	{
		return 0;
	}
}
//----------------------------------------------------------------
inline void SoArray::Clear()
{
	m_nCurrentCount = 0;
}
//----------------------------------------------------------------
inline int SoArray::GetCount() const
{
	return m_nCurrentCount;
}
//----------------------------------------------------------------
#endif //_SoArray_h_
//----------------------------------------------------------------

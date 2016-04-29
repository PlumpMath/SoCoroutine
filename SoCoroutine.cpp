//------------------------------------------------------------
/* The credit belongs to Tom Duff , Simon Tatham and zhicheng */
//http://coolshell.cn/articles/10975.html
//https://github.com/zhicheng/coroutine
//����һ��ʹ�á�switch-case��ʵ�ֵ���������ԭ�Ｖ��ķ�ɧ�����˾�̾��Э�̿⡣
//֧�ֿ�ƽ̨��֧�ֺ������루Re-entrant����
//�Զ��̵߳�֧�������δ֪��
//�������Э�̿��ʵ�ַ����쳣�򵥣�������ʹ��ʱ��һЩע�����
//1�������� SoCoroutineFuncPointer �����Ŀ�ʼ������ SoCoroutineBegin ���ڸú�����ĩβ������ SoCoroutineEnd ��
//   Ҳ������ SoCoroutineBegin-SoCoroutineEnd �����԰� SoCoroutineFuncPointer ����ס��
//2����ֹ��ͬһ�д�������������λ��߶�� SoCoroutineYield ��
//
//�����α����չʾ��ʹ�÷�����
// SoCoroutine* pCo = SoCoroutineCreate()
// ...
//    void testCo(SoCoroutine* pCo)
//    {
//      //<<<<<<<<<<<<<<<<<<<<<<<
//      //�� SoCoroutineBegin ����֮ǰ�Ĵ���飬ÿ��ִ�б���������ִ��һ�Ρ�
//      //>>>>>>>>>>>>>>>>>>>>>>>
//      //<<<<<<<<<<<<<<<<<<<<<<<
//      //�� SoCoroutineBegin ������ʼ��������ĩβ����pCo��������ִֻ��һ�Ρ�
//      SoCoroutineBegin(pCo);
//      ...
//      SoCoroutineYield(pCo);
//      ...
//      SoCoroutineEnd(pCo);
//      //>>>>>>>>>>>>>>>>>>>>>>>
//    }
// ...
//    other code invote SoCoroutineResume(pCo)
// ...
// SoCoroutineDelete(&pCo)
//------------------------------------------------------------
#include "SoCoroutine.h"
//------------------------------------------------------------
SoCoroutineManager* SoCoroutineManager::ms_pInstance = 0;
//------------------------------------------------------------
bool SoCoroutineManager::CreateCoroutineManager()
{
	bool br = true;
	if (ms_pInstance == 0)
	{
		ms_pInstance = new SoCoroutineManager;
		if (ms_pInstance && ms_pInstance->InitCoroutineManager())
		{
			br = true;
		}
		else
		{
			ReleaseCoroutineManager();
			br = false;
		}
	}
	return br;
}
//------------------------------------------------------------
void SoCoroutineManager::ReleaseCoroutineManager()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = 0;
	}
}
//----------------------------------------------------------------
SoCoroutineManager::SoCoroutineManager()
:m_nCountOfUndeadCoroutine(0)
{

}
//----------------------------------------------------------------
SoCoroutineManager::~SoCoroutineManager()
{
	ClearCoroutineManager();
}
//----------------------------------------------------------------
bool SoCoroutineManager::InitCoroutineManager()
{
	if (m_kArray.InitArray(sizeof(SoCoroutine), 10, 10) == false)
	{
		return false;
	}
	return true;
}
//------------------------------------------------------------
void SoCoroutineManager::ClearCoroutineManager()
{
	SoCoroutine* pTemp = 0;
	const int nCount = m_kArray.GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		pTemp = (SoCoroutine*)m_kArray.GetElement(i);
		if (pTemp)
		{
			pTemp->Clear();
		}
	}
	m_kArray.Clear();
	m_nCountOfUndeadCoroutine = 0;
}
//------------------------------------------------------------
void SoCoroutineManager::UpdateCoroutineManager(float fDeltaTime)
{
	if (m_nCountOfUndeadCoroutine > 0)
	{
		SoCoroutine* pCo = 0;
		const int nCount = m_kArray.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			pCo = (SoCoroutine*)m_kArray.GetElement(i);
			if (pCo)
			{
				if (pCo->m_nStatus == SoCoroutineStatus_Suspend)
				{
					if (pCo->m_fRemainWaitTime > 0.0f)
					{
						pCo->m_fRemainWaitTime -= fDeltaTime;
						if (pCo->m_fRemainWaitTime <= 0.0f)
						{
							pCo->m_fRemainWaitTime = -1.0f;
							SoCoroutineResume(pCo);
						}
					}
				}
			}
		}
	}
}
//------------------------------------------------------------
SoCoroutine* SoCoroutineManager::CreateCoroutine(SoCoroutineFuncPointer pFunc, void* pUserData)
{
	SoCoroutine* pCo = FindEmptyElement();
	if (pCo == 0)
	{
		SoCoroutine kCo;
		const int nIndex = m_kArray.PushBack(&kCo);
		pCo = (SoCoroutine*)m_kArray.GetElement(nIndex);
	}
	if (pCo)
	{
		pCo->m_nStatus = SoCoroutineStatus_Begin;
		pCo->m_pFunc = pFunc;
		pCo->m_pUserData = pUserData;
		++m_nCountOfUndeadCoroutine;
	}
	return pCo;
}
//------------------------------------------------------------
void SoCoroutineManager::DeleteCoroutine(SoCoroutine** ppCo)
{
	if (ppCo && *ppCo)
	{
		if ((*ppCo)->m_nStatus != SoCoroutineStatus_Dead)
		{
			--m_nCountOfUndeadCoroutine;
		}
		(*ppCo)->Clear();
		(*ppCo) = 0;
	}
}
//------------------------------------------------------------
SoCoroutine* SoCoroutineManager::FindEmptyElement()
{
	SoCoroutine* pCo = 0;
	SoCoroutine* pTemp = 0;
	const int nCount = m_kArray.GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		pTemp = (SoCoroutine*)m_kArray.GetElement(i);
		if (pTemp && pTemp->m_nStatus == SoCoroutineStatus_Dead)
		{
			pCo = pTemp;
			break;
		}
	}
	return pCo;
}
//------------------------------------------------------------

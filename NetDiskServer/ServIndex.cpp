// ServIndex.cpp : implementation file
//�������ļ�ϵͳ������

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServIndex.h"
#include "locale.h"

// CServIndex

CServIndex::CServIndex()
{
	m_strRootPath=_T("E:\\��ҵ����Ŀ¼\\");
	m_strIndexPath=_T("E:\\��ҵ����Ŀ¼\\��������Ŀ¼\\");
	m_catalogIndexHead=new CCatalogIndex();
	
}

CServIndex::~CServIndex()
{
	if(m_catalogIndexHead!=NULL)
		delete m_catalogIndexHead;
}

//��ȡ��Ŀ¼��
int CServIndex::getSubCatalogCount(CCatalogIndex* catalogHead)
{
	int count=0;
	CCatalogIndex* head=catalogHead;
	

	while (head!=NULL)
	{
		count++;
		head=head->m_subCatalog;
	}

	return count;
}
//��ȡͬ��Ŀ¼��
int CServIndex::getNextCatalogCount(CCatalogIndex* catalogHead)
{
	int count=0;
	CCatalogIndex* head=catalogHead;

	while(head !=NULL)
	{
		count++;
		head=head->m_NextCatalog;
	}
	return count;
}
//��ȡ���ļ���
int CServIndex::getSubFileCount(CFileIndex* fileHead)
{
	int count=0;

	CFileIndex* head=fileHead;

	while(NULL != head)
	{
		count++;
		head=head->m_Next;
	}

	return count;
}

//д��Ŀ¼������Ϣ
void CServIndex::writeSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=getSubCatalogCount(catalogHead);
	CArchive carchive(pcFile,CArchive::store);
	carchive<<count;

	while(catalogHead !=NULL)
	{
		carchive<<catalogHead->m_strName<<catalogHead->m_strPath<<catalogHead->m_strEditTime<<catalogHead->m_bIsShare<<catalogHead->m_strSharePath;
		//writeSubCatalogInfo(catalogHead->m_subCatalog,pcFile);
		catalogHead=catalogHead->m_NextCatalog;
	}

}

//д���ļ�������Ϣ
void CServIndex::writeSubFileInfo(CFileIndex* fileIndex,CFile* pcFile)
{
	int count=getSubFileCount(fileIndex);
	CArchive carchive(pcFile,CArchive::store);
	carchive<<count;

	while(fileIndex != NULL)
	{
		carchive<<fileIndex->m_strName<<fileIndex->m_strPath<<fileIndex->m_strEditTime<<fileIndex->m_bIsShare<<fileIndex->m_strSharePath;


		fileIndex=fileIndex->m_Next;
	}
}

//д��ͬ��Ŀ¼��Ϣ
void CServIndex::writeNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=getNextCatalogCount(catalogHead);
	CArchive carchive(pcFile,CArchive::store);
	carchive<<count;

	while(catalogHead !=NULL)
	{
		carchive<<catalogHead->m_strName<<catalogHead->m_strPath<<catalogHead->m_strEditTime<<catalogHead->m_bIsShare<<catalogHead->m_strSharePath;
		writeSubFileInfo(catalogHead->m_subFile,pcFile);
		writeSubCatalogInfo(catalogHead->m_subCatalog,pcFile);

		catalogHead=catalogHead->m_NextCatalog;
	}
}
void CServIndex::testWriteFile(CFileIndex* fileHead,CStdioFile* pStdFile)
{
	int count=getSubFileCount(fileHead);
	//CString tmp;
	//tmp.Format(_T("%d"),count);
	//pStdFile->WriteString(tmp+_T("-"));

	while(fileHead!=NULL)
	{
		pStdFile->WriteString(fileHead->m_strName+_T("-"));
		pStdFile->WriteString(fileHead->m_strPath+_T("-"));
		pStdFile->WriteString(fileHead->m_strEditTime+_T("-"));
		if(fileHead->m_bIsShare)
			pStdFile->WriteString(_T("TRUE-"));
		else
			pStdFile->WriteString(_T("FALSE-"));
		pStdFile->WriteString(fileHead->m_strSharePath+_T("-"));
		pStdFile->WriteString(_T("\n"));
		fileHead=fileHead->m_Next;
	}

}
void CServIndex::testWriteSub(CCatalogIndex* catalogHead,CStdioFile* pStdFile)
{
	int count=getSubCatalogCount(catalogHead);
	//CString tmp;
	//tmp.Format(_T("%d"),count);
	//pStdFile->WriteString(tmp+_T("-"));
	
	while(catalogHead!=NULL)
	{
		pStdFile->WriteString(catalogHead->m_strName+_T("-"));
		pStdFile->WriteString(catalogHead->m_strPath+_T("-"));
		pStdFile->WriteString(catalogHead->m_strEditTime+_T("-"));
		if(catalogHead->m_bIsShare)
			pStdFile->WriteString(_T("TRUE-"));
		else
			pStdFile->WriteString(_T("FALSE-"));
		pStdFile->WriteString(catalogHead->m_strSharePath+_T("-"));
		pStdFile->WriteString(_T("\n"));
		
		testWriteSub(catalogHead->m_subCatalog,pStdFile);
		testWriteFile(catalogHead->m_subFile,pStdFile);

		catalogHead= catalogHead->m_subCatalog;
	}

}
void CServIndex::testWriteNext(CCatalogIndex* catalogHead,CStdioFile* pStdFile)
{
	int count=getNextCatalogCount(catalogHead);
	//CString tmp;
	//tmp.Format(_T("%d"),count);
	//pStdFile->WriteString(tmp+_T("-"));

	while(catalogHead!=NULL)
	{
		pStdFile->WriteString(catalogHead->m_strName+_T("-"));
		pStdFile->WriteString(catalogHead->m_strPath+_T("-"));
		pStdFile->WriteString(catalogHead->m_strEditTime+_T("-"));
		if(catalogHead->m_bIsShare)
			pStdFile->WriteString(_T("TRUE-"));
		else
			pStdFile->WriteString(_T("FALSE-"));
		pStdFile->WriteString(catalogHead->m_strSharePath+_T("-"));
		pStdFile->WriteString(_T("\n"));

		testWriteNext(catalogHead->m_subCatalog,pStdFile);
		testWriteFile(catalogHead->m_subFile,pStdFile);
		catalogHead=catalogHead->m_NextCatalog;
	}

}
//��ȡ��Ŀ¼������Ϣ
void CServIndex::readSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=0;
	CCatalogIndex* head=catalogHead;
	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;	
		//readSubFileInfo(head->m_subFile,pcFile);
		CCatalogIndex* newCatalogIndex=new CCatalogIndex();
		head->m_subCatalog=newCatalogIndex;
		head=newCatalogIndex;
	}
}

//��ȡ���ļ�������Ϣ
void CServIndex::readSubFileInfo(CFileIndex* fileHead,CFile* pcFile)
{
	int count=0;
	CFileIndex* head=fileHead;

	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;
		CFileIndex* newFileIndex=new CFileIndex();
		head->m_Next=newFileIndex;
		head=newFileIndex;
	}
}

//��ȡͬ��Ŀ¼��Ϣ
void CServIndex::readNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=0;
	CCatalogIndex* head=catalogHead;

	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;	
		readSubCatalogInfo(head->m_subCatalog,pcFile);
		readSubFileInfo(head->m_subFile,pcFile);
		CCatalogIndex* newCatalogIndex=new CCatalogIndex();
		head->m_NextCatalog=newCatalogIndex;
		head=newCatalogIndex;
	}
}

//�����û������ļ�
BOOL CServIndex::CreateIndex(CString rootName)
{
	CString path=m_strIndexPath;
	path+=rootName+_T(".txt");

	setlocale( LC_CTYPE, ("chs"));
	CStdioFile pStdioFile;
	if(pStdioFile.Open(path,CFile::modeCreate|CFile::modeWrite))
	{

		//����������Ϣ
		UpdateIndex(m_strRootPath+rootName,m_catalogIndexHead);
		testWriteNext(m_catalogIndexHead->m_subCatalog,&pStdioFile);
		AfxMessageBox(_T("�����ļ������ɹ�!"));

		pStdioFile.Close();
		return TRUE;
	}
	//CFile file;
	//if(file.Open(path,CFile::modeCreate|CFile::modeWrite))
	//{
	//	//����������Ϣ
	//	UpdateIndex(m_strRootPath+rootName,m_catalogIndexHead);
	//	writeNextCatalogInfo(m_catalogIndexHead->m_subCatalog,&file);
	//	AfxMessageBox(_T("�����ļ������ɹ�!"));
	//	file.Close();
	//	return TRUE;
	//}

	return FALSE;
}

//�����û�Ŀ¼�����ļ�
void CServIndex::UpdateIndex(CString rootPath,CCatalogIndex* catalogIndexHead)
{
	BuildSubCatalogAndFileList(rootPath,catalogIndexHead);
	CCatalogIndex* tmp=catalogIndexHead->m_subCatalog;

	while(tmp!=NULL)
	{
		RecurBuildCatalogList(tmp->m_strPath,tmp);
		tmp=tmp->m_NextCatalog;
	}
}

//�ݹ齨����Ŀ¼����
void CServIndex::RecurBuildCatalogList(CString path,CCatalogIndex* rootIndex)
{
	CFileFind finder;
	BOOL isFound;
	CString strTmpDir;		//����ļ���·��
	CTime tmpTime;
	CString tmpStr;
	CCatalogIndex* tmpSubCatIndex=rootIndex->m_subCatalog;
	isFound=finder.FindFile(path+_T("\\*.*"));

	BuildSubCatalogAndFileList(path,rootIndex);

	while(isFound)
	{
		isFound=finder.FindNextFile();
		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
		{
			//strTmpDir=_T("");
			strTmpDir=finder.GetFilePath();

			if(IsDirecEmpty(strTmpDir))
				return;
			//������һ��Ŀ¼�����ڵ�
			CCatalogIndex* CatalogIndexItem=new CCatalogIndex();
			CatalogIndexItem->m_strName=finder.GetFileName();
			CatalogIndexItem->m_strPath=strTmpDir;
			finder.GetLastAccessTime(tmpTime);
			tmpStr = tmpTime.Format(_T("%c"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
			CatalogIndexItem->m_strEditTime=tmpStr;
			CatalogIndexItem->m_bIsShare=FALSE;
			CatalogIndexItem->m_strSharePath=_T("");
			CatalogIndexItem->m_subFile=NULL;
			CatalogIndexItem->m_NextCatalog=NULL;

			CatalogIndexItem->m_subCatalog=rootIndex->m_subCatalog;
			rootIndex->m_subCatalog=CatalogIndexItem;
			RecurBuildCatalogList(strTmpDir,rootIndex->m_subCatalog);
		}
	}
	//rootIndex->m_subCatalog=tmpSubCatIndex;
}

//����ͬ��Ŀ¼�����ļ�������
void CServIndex::BuildSubCatalogAndFileList(CString rootPath,CCatalogIndex* rootIndex)
{
	CFileFind finder;
	BOOL isFounded;
	int iDirecCount=0;
	int iFileCount=0;
	//bool c_flag=false;
	//bool f_flag=false;
	CString strTmpDir;		//����ļ���·��
	CTime tmpTime;
	CString tmpStr;

	if(NULL == rootIndex)
		return ;
	if(IsDirecEmpty(rootPath))
		return ;
	//CCatalogIndex* tmpCatIndex=rootIndex->m_subCatalog;
	CCatalogIndex* tmpCatalogIndexHead=new CCatalogIndex();
	//CFileIndex* tmpFileIndex=rootIndex->m_subFile;
	CFileIndex* tmpFileIndexHead=new CFileIndex();
	

	isFounded=finder.FindFile(rootPath+_T("\\*.*"));

	while(isFounded)
	{
		isFounded=finder.FindNextFile();

		if(finder.IsDots())
			continue ;
		if(finder.IsDirectory())
		{
			iDirecCount++;
			strTmpDir=_T("");
			strTmpDir=finder.GetFilePath();
			//������һ��Ŀ¼�����ڵ�
			CCatalogIndex* CatalogIndexItem=new CCatalogIndex();
			CatalogIndexItem->m_strName=finder.GetFileName();
			CatalogIndexItem->m_strPath=strTmpDir;
			finder.GetLastAccessTime(tmpTime);
			tmpStr = tmpTime.Format(_T("%c"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
			CatalogIndexItem->m_strEditTime=tmpStr;
			CatalogIndexItem->m_bIsShare=FALSE;
			CatalogIndexItem->m_strSharePath=_T("");
			CatalogIndexItem->m_subFile=NULL;
			CatalogIndexItem->m_NextCatalog=NULL;

			//���뵽��Ŀ¼����
			if(tmpCatalogIndexHead->m_strName==_T(""))
				tmpCatalogIndexHead=CatalogIndexItem;
			else
			{
				CatalogIndexItem->m_NextCatalog=tmpCatalogIndexHead->m_NextCatalog;
				tmpCatalogIndexHead->m_NextCatalog=CatalogIndexItem;
			}
			//CatalogIndexItem->m_NextCatalog=tmpCatIndex;
			//tmpCatIndex=CatalogIndexItem;
			//if(!c_flag)
			//{
			//	tmpCatalogIndexHead=CatalogIndexItem;
			//	c_flag=true;
			//}


		}
		else
		{
			iFileCount++;
			//���ӵ�ǰ�û��ļ����������ڵ�
			CFileIndex* fileIndexItem=new CFileIndex();
			fileIndexItem->m_strName=finder.GetFileName();
			fileIndexItem->m_strPath=finder.GetFilePath();
			finder.GetLastAccessTime(tmpTime);
			tmpStr = tmpTime.Format(_T("%c"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
			fileIndexItem->m_strEditTime=tmpStr;
			fileIndexItem->m_bIsShare=FALSE;
			fileIndexItem->m_strSharePath=_T("");

			//���뵽���ļ�����
			if(_T("") == tmpFileIndexHead->m_strName)
			{
				tmpFileIndexHead=fileIndexItem;
			}
			else
			{
				fileIndexItem->m_Next=tmpFileIndexHead->m_Next;
				tmpFileIndexHead->m_Next=fileIndexItem;
			}
			//fileIndexItem->m_Next=tmpFileIndex;
			//tmpFileIndex=fileIndexItem;
			//if(!f_flag)
			//{
			//	tmpFileIndexHead=fileIndexItem;
			//	f_flag=true;
			//}
		}
	}
	if(iDirecCount!=0)
		rootIndex->m_subCatalog=tmpCatalogIndexHead;
	else
		rootIndex->m_subCatalog=NULL;
	if(iFileCount!=0)
		rootIndex->m_subFile=tmpFileIndexHead;
	else
		rootIndex->m_subFile=NULL;
}

//�жϵ�ǰĿ¼�Ƿ�Ϊ��
BOOL CServIndex::IsDirecEmpty(CString path)
{
	CFileFind finder;
	int count=0;
	BOOL isWork;
	isWork=finder.FindFile(path+_T("\\*.*"));

	while(isWork)
	{
		isWork=finder.FindNextFile();
		if(finder.IsDots())
			continue;
		count++;
	}
	return (count>0?FALSE:TRUE);
}
// CServIndex member functions
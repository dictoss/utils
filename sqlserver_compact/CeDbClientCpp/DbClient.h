#ifndef __DBCLIENT_H__
#define __DBCLIENT_H__

#include "stdafx.h"
#include "DbDataReader.h"

/*****************************************************************************
 * DbClient �N���X
 *   �f�[�^�x�[�X�փA�N�Z�X����N���C�A���g�N���X�i���ۃN���X�j
 *****************************************************************************/
class DbClient
{
/********************
 * �C���^�t�F�[�X
 ********************/
public:
	/* �R���X�g���N�^ */
	DbClient(void);
	DbClient(const TCHAR *dsn);
	
	/* �f�X�g���N�^ */
	virtual ~DbClient(void) = 0;

	/* �f�[�^�x�[�X�Ƃ̐ڑ����J�� */
	virtual int open() = 0;
	virtual int open(const TCHAR *dsn) = 0;
	virtual bool is_open() = 0;

	/* �f�[�^�x�[�X�Ƃ̐ڑ������ */
	virtual void close() = 0;

	/* �N�G�������s���� */
	virtual LONG execute(const TCHAR *query) = 0;
	virtual LONG execute_scaler(const TCHAR *query) = 0;
	virtual DbDataReader* exetute_select(const TCHAR *query) = 0;

	/* DataReader�����B */
	//virtual void close_datareader(DbDataReader *reader) = 0;

	/* �g�����U�N�V���� */
	virtual int begin_tran( int level ) = 0;
	virtual int commit_tran() = 0;
	virtual int rollback_tran() = 0;

public:
	/* �ڑ������� */
	TCHAR m_dsn[512];

protected:
	/* �f�[�^�x�[�X�Ɛڑ����Ă��邩�𔻒� */
	bool m_IsOpened;
};

#if defined(_WIN32_WCE) || defined(_WIN32)
/*****************************************************************************
 * CDbSqlCeClient �N���X
 *   SQL Server Compact�փA�N�Z�X����N���C�A���g�N���X
 *   �f�[�^�x�[�X�ɂ́AOLE DB��p���ăA�N�Z�X���܂��B
 *   ���̂��߁A�Ăяo���O�ɁACoInitialize()���Ăяo���A�I���O��CoUninitialize()��
 *   �Ăяo���Ďg���Ă��������B
 *****************************************************************************/
class DbSqlCeClient : public DbClient
{
/********************
 * �C���^�t�F�[�X
 ********************/
public:
	/* �R���X�g���N�^ */
	DbSqlCeClient(void);
	DbSqlCeClient(const TCHAR *dsn);
	
	/* �f�X�g���N�^ */
	virtual ~DbSqlCeClient(void);

	/* �f�[�^�x�[�X�Ƃ̐ڑ����J�� */
	virtual int open();
	virtual int open(const TCHAR *dsn);
	bool is_open();

	/* �f�[�^�x�[�X�Ƃ̐ڑ������ */
	void close();

	/* �N�G�������s���� */
	LONG execute(const TCHAR *query);
	LONG execute_scaler(const TCHAR *query);
	DbSqlCeDataReader* exetute_select(const TCHAR *query);

	/* DataReader�����B */
	void close_datareader(DbSqlCeDataReader *reader);

	/* �g�����U�N�V���� */
	int begin_tran( int level );
	int begin_tran( ISOLEVEL level );
	int commit_tran();
	int rollback_tran();

protected:
	int create_open_prop( DBPROPSET *parPropset, int size );

/********************
 * ����
 ********************/
public:
	DbSqlCeDataReader *m_pReader;
	GUID m_sqlceVerGUID;
	
protected:
	typedef struct{
		TCHAR *key;
		VARTYPE type;
		ULONG reqType;
		GUID PropSetId;
		ULONG ulPropId;
	} RINIT_PROP;

	IDBInitialize     *m_pDBinit;
	IDBCreateSession  *m_pSession;
	IDBCreateCommand  *m_pCrtCmd;
	ICommandText      *m_pCmdtext;
	ITransactionLocal *m_pTran;

/********************
 * �萔�l
 ********************/
	static const RINIT_PROP m_properties[];
	static const int PROPSET_MAX;

};
#endif

#endif /* __DBCLIENT_H__ */

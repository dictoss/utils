#ifndef __DBCLIENT_H__
#define __DBCLIENT_H__

#include "stdafx.h"
#include "DbDataReader.h"

/*****************************************************************************
 * DbClient クラス
 *   データベースへアクセスするクライアントクラス（抽象クラス）
 *****************************************************************************/
class DbClient
{
/********************
 * インタフェース
 ********************/
public:
	/* コンストラクタ */
	DbClient(void);
	DbClient(const TCHAR *dsn);
	
	/* デストラクタ */
	virtual ~DbClient(void) = 0;

	/* データベースとの接続を開く */
	virtual int open() = 0;
	virtual int open(const TCHAR *dsn) = 0;
	virtual bool is_open() = 0;

	/* データベースとの接続を閉じる */
	virtual void close() = 0;

	/* クエリを実行する */
	virtual LONG execute(const TCHAR *query) = 0;
	virtual LONG execute_scaler(const TCHAR *query) = 0;
	virtual DbDataReader* exetute_select(const TCHAR *query) = 0;

	/* DataReaderを閉じる。 */
	//virtual void close_datareader(DbDataReader *reader) = 0;

	/* トランザクション */
	virtual int begin_tran( int level ) = 0;
	virtual int commit_tran() = 0;
	virtual int rollback_tran() = 0;

public:
	/* 接続文字列 */
	TCHAR m_dsn[512];

protected:
	/* データベースと接続しているかを判定 */
	bool m_IsOpened;
};

#if defined(_WIN32_WCE) || defined(_WIN32)
/*****************************************************************************
 * CDbSqlCeClient クラス
 *   SQL Server Compactへアクセスするクライアントクラス
 *   データベースには、OLE DBを用いてアクセスします。
 *   そのため、呼び出す前に、CoInitialize()を呼び出し、終了前にCoUninitialize()を
 *   呼び出して使ってください。
 *****************************************************************************/
class DbSqlCeClient : public DbClient
{
/********************
 * インタフェース
 ********************/
public:
	/* コンストラクタ */
	DbSqlCeClient(void);
	DbSqlCeClient(const TCHAR *dsn);
	
	/* デストラクタ */
	virtual ~DbSqlCeClient(void);

	/* データベースとの接続を開く */
	virtual int open();
	virtual int open(const TCHAR *dsn);
	bool is_open();

	/* データベースとの接続を閉じる */
	void close();

	/* クエリを実行する */
	LONG execute(const TCHAR *query);
	LONG execute_scaler(const TCHAR *query);
	DbSqlCeDataReader* exetute_select(const TCHAR *query);

	/* DataReaderを閉じる。 */
	void close_datareader(DbSqlCeDataReader *reader);

	/* トランザクション */
	int begin_tran( int level );
	int begin_tran( ISOLEVEL level );
	int commit_tran();
	int rollback_tran();

protected:
	int create_open_prop( DBPROPSET *parPropset, int size );

/********************
 * 属性
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
 * 定数値
 ********************/
	static const RINIT_PROP m_properties[];
	static const int PROPSET_MAX;

};
#endif

#endif /* __DBCLIENT_H__ */

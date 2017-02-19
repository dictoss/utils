#ifndef __DBDATAREADER_H__
#define __DBDATAREADER_H__

#include "stdafx.h"


/*****************************************************************************
 * DbDataReader クラス
 *   Select結果を読み取りするクラス（抽象クラス）
 *****************************************************************************/
class DbDataReader
{
public:
	typedef enum{
		SEEK_OK,
		SEEK_EOF,
		SEEK_ERROR,
	} SEEK_RESULT;

	/* コンストラクタ */
	DbDataReader(void);
	/* デストラクタ */
	virtual ~DbDataReader(void);

	/*  */
	virtual SEEK_RESULT read() = 0;
	virtual SEEK_RESULT prev_read() = 0;
	virtual SEEK_RESULT seek( int pos ) = 0;
	virtual bool is_eof() = 0;

	virtual int get_column( unsigned int index, void *buf, size_t size, int type ) = 0;

public:
	bool m_isEof;

};


#if defined(_WIN32_WCE) || defined(_WIN32)
/*****************************************************************************
 * DbSqlCeDataReader クラス
 *   SQL Server CompactのSelect結果を読み取りするクラス
 *****************************************************************************/
class DbSqlCeDataReader : public DbDataReader
{
/**********************
 * 定数定義
 **********************/
public:
	static const ULONG dblength_size;

/**********************
 * インタフェース
 **********************/
public:
	DbSqlCeDataReader(void);
	DbSqlCeDataReader(IRowsetPosition *setpos);
	virtual ~DbSqlCeDataReader(void);

	/*  */
	virtual SEEK_RESULT read();
	virtual SEEK_RESULT prev_read();
	virtual SEEK_RESULT seek( int pos );
	virtual bool is_eof();

	bool init( IRowsetPosition *setpos );
	bool init();

	virtual int get_column( unsigned int index, void *buf, size_t size, int type );

/*********************
 * 属性
 *********************/
public:
	bool m_IsInit;
	ULONG m_RowCount;
	ULONG m_ColCount;

protected:
	IRowsetPosition *m_pRowsetPos;
	IMalloc         *m_pMalloc;

	IRowset         *m_pRowset;
	IAccessor       *m_pAccessor;
	IColumnsInfo	*m_pColInfo;
	DBCOLUMNINFO	*m_pDbColInfo;

	DBBINDING		*m_pBind;

	HACCESSOR       m_hAccessor;
	HROW            m_hRows[1];
	HROW            *m_pRowHeader;

	BYTE           *m_pRowData;
	ULONG          m_ulBufferSize;
	bool           m_bRowDataEnable;

};
#endif


#endif /* __DBDATAREADER_H__ */

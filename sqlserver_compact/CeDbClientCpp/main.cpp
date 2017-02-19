// TestCeDbCpp1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DbClient.h"
#include "DbDataReader.h"

/*****************************************************************************
 * データ型
 *****************************************************************************/
typedef struct {
    int id;
    WCHAR name[250 + 1];
    WCHAR type[2 + 1];
    int price;
} RTB_THINGS;


/*****************************************************************************
 * プロトタイプ
 *****************************************************************************/
int init( int argc, TCHAR **argv );
void cleanup();
int ProcMain();

/*****************************************************************************
 * グローバル変数
 *****************************************************************************/
int g_argc;
TCHAR **g_argv;


/*****************************************************************************
 * main
 *****************************************************************************/
int _tmain(int argc, TCHAR* argv[])
{
	int ret;

	ret = init( argc, argv );
	if( -1 == ret ){
		return 1;
	}

	/*  */
	ProcMain();

	cleanup();

	return 0;
}


/*****************************************************************************
 * init
 *****************************************************************************/
int init( int argc, TCHAR *argv[] )
{
	HRESULT hr;

	hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
	if( FAILED( hr ) ){
		return -1;
	}

	g_argc = argc;
	g_argv = argv;

	return 0;
}


/*****************************************************************************
 * cleanup
 *****************************************************************************/
void cleanup()
{
	CoUninitialize();
}


/*****************************************************************************
 * ProcMain()
 *****************************************************************************/
int ProcMain()
{
	DbSqlCeClient sqlce;
	DbDataReader::SEEK_RESULT seekret;

	TCHAR szConnStr[] = _T("DataSource=\\Storage Card\\oledbtest.sdf;");
	int ret;

    RTB_THINGS oneRecord;

    memset( &oneRecord, 0, sizeof(oneRecord) );

	sqlce.m_sqlceVerGUID = CLSID_SQLSERVERCE_3_0;
	
	ret = sqlce.open( szConnStr );
	if( 0 != ret ){
		return -1;
	}

	/*
	ret = sqlce.execute( _T("INSERT INTO t_man ( id, name ) VALUES ( 2, N\'bbb\' );") );
	if( -1 < ret ){
		ret = ret;
	}
	*/

    TCHAR *query = _T("SELECT id, name, type, price FROM t_thing;");
	DbSqlCeDataReader *pReader = sqlce.exetute_select( query );
	if( NULL == pReader ){
		ret = ret;	
	}
	else{
        while( DbDataReader::SEEK_OK == (seekret = pReader->read()) ){
            pReader->get_column( 0, (void*)&oneRecord.id, sizeof(oneRecord.id), DBTYPE_I4 );
            pReader->get_column( 1, (void*)oneRecord.name, sizeof(oneRecord.name), DBTYPE_WSTR );
            pReader->get_column( 2, (void*)oneRecord.type, sizeof(oneRecord.type), DBTYPE_WSTR );
            pReader->get_column( 3, (void*)&oneRecord.price, sizeof(oneRecord.price), DBTYPE_I4 );

            // データチェック
            oneRecord = oneRecord;
        }

		if( seekret == DbDataReader::SEEK_ERROR ){
			seekret = DbDataReader::SEEK_ERROR;
		}

        while( DbDataReader::SEEK_OK == (seekret = pReader->prev_read()) ){
            pReader->get_column( 0, (void*)&oneRecord.id, sizeof(oneRecord.id), DBTYPE_I4 );
            pReader->get_column( 1, (void*)oneRecord.name, sizeof(oneRecord.name), DBTYPE_WSTR );
            pReader->get_column( 2, (void*)oneRecord.type, sizeof(oneRecord.type), DBTYPE_WSTR );
            pReader->get_column( 3, (void*)&oneRecord.price, sizeof(oneRecord.price), DBTYPE_I4 );

            // データチェック
            oneRecord = oneRecord;
        }

        sqlce.close_datareader( pReader );
	}

	sqlce.close();


	return 0;
}

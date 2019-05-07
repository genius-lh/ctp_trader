/**
 * ��Ȩ����(C)2012-2016, �����ɴ���Ϣ�������޹�˾
 * �ļ����ƣ�DFITCSECMdApi.h
 * �ļ�˵��������XSpeed����API�ӿ�
 * ��ǰ�汾��1.6.40
 * ���ߣ�XSpeed֤ȯ��Ŀ��
 * �������ڣ�2016��10��
 */

#ifndef DFITCSECMDAPI_H_
#define DFITCSECMDAPI_H_


#include "DFITCSECApiStruct.h"


#ifdef WIN32
    #ifdef DFITCSECAPI_EXPORTS
        #define DFITCSEC_MD_API __declspec(dllexport)
    #else
        #define DFITCSEC_MD_API __declspec(dllimport)
    #endif//DFITCAPI_EXPORTS
#else
    #define DFITCSEC_MD_API 
#endif//WIN32

    class DFITCSECMdSpi
    {
    public:
        DFITCSECMdSpi(){}

        /** 
         * ��������������Ӧ
         */
        virtual void OnFrontConnected(){};

        /**
         * �������Ӳ�������Ӧ
         */
        virtual void OnFrontDisconnected(int nReason){};
        
        /**
         * SEC-��Ϣ֪ͨ
         */
        virtual void OnRtnNotice(DFITCSECRspNoticeField *pNotice) {};

        /**
         * ERR-����Ӧ��
         * @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
         */
        virtual void OnRspError(struct DFITCSECRspInfoField *pRspInfo) {}

        /**
         * STOCK-��¼��Ӧ
         * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
         * @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
         */
        virtual void OnRspStockUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * STOCK-�ǳ���Ӧ
         * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
         */
        virtual void OnRspStockUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * SOP-��¼��Ӧ
         * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
         */
        virtual void OnRspSOPUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * SOP-�ǳ���Ӧ
         * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
         */
        virtual void OnRspSOPUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * FASL-��¼��Ӧ
         * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
         */
        virtual void OnRspFASLUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * FASL-�ǳ���Ӧ
         * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
         */
        virtual void OnRspFASLUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo) {}
       
        /**
         * STOCK-���鶩����Ӧ
         * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
         */
        virtual void OnRspStockSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * STOCK-ȡ������������Ӧ
         * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
         */
        virtual void OnRspStockUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo) {}
        
        /**
         * SOP-���鶩����Ӧ
         * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
         */
        virtual void OnRspSOPSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * SOP-ȡ������������Ӧ
         * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
         */
        virtual void OnRspSOPUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo) {}

        /**
         * STOCK-����������Ӧ
         * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
         */
        virtual void OnStockMarketData(struct DFITCStockDepthMarketDataField * pMarketDataField) {}

        /**
         * SOP-����������Ӧ
         * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
         */
        virtual void OnSOPMarketData(struct DFITCSOPDepthMarketDataField * pMarketDataField) {}

        /**
         * STOCK-����������Ӧ
         * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
         * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
         */
        virtual void OnRspStockAvailableQuot(struct DFITCRspQuotQryField * pAvailableQuotInfo, struct DFITCSECRspInfoField * pRspInfo,bool flag) {}

        /**
         * SOP-����������Ӧ
         * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
         * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
         */
        virtual void OnRspSopAvailableQuot(struct DFITCRspQuotQryField * pAvailableQuotInfo, struct DFITCSECRspInfoField * pRspInfo,bool flag) {}

        /**
         * �������������Ӧ
         * @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
         * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
         */
        virtual void OnRspUserMDPasswordUpdate(struct DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,struct DFITCSECRspInfoField * pRspInfo){}

    };//end of DFITCSECMdSpi

    class DFITCSEC_MD_API DFITCSECMdApi
    {
    public:
        /**
         * ��������APIʵ��
         * @pszLogAddr: log���ڵ�·�������pszLogAddrΪNULL��������log��
         * @return ��������UserApi
         */
        static DFITCSECMdApi * CreateDFITCMdApi(const char* pszLogAddr = "");

        /**
         * ����һϵ�г�ʼ������:ע��ص������ӿ�,��������ǰ�á�
         * @param pszSvrAddr:����ǰ�������ַ��
         *                  �����ַ�ĸ�ʽΪ:"protocol://ipaddress:port",��"tcp://127.0.0.1:10915"
         *                  ����protocol��ֵΪtcp����udp,��ʾ��������ķ�ʽ;�����udp������������,udp�Ķ˿ڽ���API����ȷ����
         *                  ipaddress��ʾ����ǰ�õ�IP,port��ʾ����ǰ�õĶ˿�
         * @param *pSpi:��DFITCMdSpi����ʵ��
         * @return 0 - �ɹ�;-1 - ʧ�ܡ�
         */
        virtual int Init(const char * pszSvrAddr, DFITCSECMdSpi * pSpi) = 0;

        /**
         * ɾ���ӿڶ���
         */
        virtual void Release() = 0;

        /**
         * STOCK-������������
         * @param ppInstrumentID[]:ָ�����飬ÿ��ָ��ָ��һ����Լ  
         * @param nCount:��Լ����
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int SubscribeStockMarketData(char * ppInstrumentID[], int nCount, int nRequestID) = 0;

        /**
         * STOCK-ȡ��������������
         * @param ppInstrumentID[]:ָ�����飬ÿ��ָ��ָ��һ����Լ
         * @param nCount:��Լ����
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int UnSubscribeStockMarketData(char * ppInstrumentID[], int nCount, int nRequestID) = 0;

        /**
         * SOP-������������
         * @param ppInstrumentID[]:ָ�����飬ÿ��ָ��ָ��һ����Լ
         * @param nCount:��Լ����
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int SubscribeSOPMarketData(char * ppInstrumentID[], int nCount, int nRequestID) = 0;

        /**
         * SOP-ȡ��������������
         * @param ppInstrumentID[]:ָ�����飬ÿ��ָ��ָ��һ����Լ
         * @param nCount:��Լ����
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int UnSubscribeSOPMarketData(char * ppInstrumentID[], int nCount, int nRequestID) = 0;

        /**
         * STOCK-��¼����
         * @param pReqUserLoginField:ָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */ 
        virtual int ReqStockUserLogin(struct DFITCSECReqUserLoginField * pReqUserLoginField) = 0;

        /**
         * STOCK-�ǳ�����
         * @param pReqUserLogoutFieldָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */ 
        virtual int ReqStockUserLogout(struct DFITCSECReqUserLogoutField * pReqUserLogoutField) = 0;

        /**
         * SOP-��¼����
         * @param ReqStockUserLogin:ָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int ReqSOPUserLogin(struct DFITCSECReqUserLoginField * pReqUserLoginField) = 0;

        /**
         * FASL-��¼����
         * @param ReqStockUserLogin:ָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int ReqFASLUserLogin(struct DFITCSECReqUserLoginField * pReqUserLoginField) = 0;
        
        /**
         * SOP-�ǳ�����
         * @param pReqUserLogoutField:ָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int ReqSOPUserLogout(struct DFITCSECReqUserLogoutField * pReqUserLogoutField) = 0;

        /**
         * FASL-�ǳ�����
         * @param pReqUserLogoutField:ָ���û���¼����ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */
        virtual int ReqFASLUserLogout(struct DFITCSECReqUserLogoutField * pReqUserLogoutField) = 0;

        /**
         * STOCK-����������Ϣ��ѯ����
         * @param pReqQuotQryField:����������������ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */ 
        virtual int ReqStockAvailableQuotQry(struct  APISTRUCT DFITCReqQuotQryField * pReqQuotQryField) = 0;

        /**
         * SOP-����������Ϣ��ѯ����
         * @param pReqQuotQryField:����������������ṹ�ĵ�ַ
         * @return 0��ʾ�ɹ���-1��ʾʧ��
         */ 
        virtual int ReqSopAvailableQuotQry(struct  APISTRUCT DFITCReqQuotQryField * pReqQuotQryField) = 0;

        /**
         * �����������
         * @param pReqMDPasswordUpdate:ָ���û������������ṹ�ĵ�ַ
         * @return 0��ʾ�����ͳɹ�������ֵ��ʾ������ʧ�ܣ�������������error.xml
         */
        virtual int ReqUserMDPasswordUpdate(struct DFITCSECReqMDPasswordUpdateField *pReqMDPasswordUpdate) = 0;

    protected:
        virtual ~DFITCSECMdApi() = 0;

    };//end of DFITCSECMdApi

#endif//DFITCSECMDAPI_H_

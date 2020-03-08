#include "rtpH264Analyzer.h"


#define H264_STATRTCODE_LEN 4
static const char H264_STATRTCODE_ADDR[4] = { 0x00,0x00,0x00,0x01 };

#define SAFE_COPY_VIDEO(videobuffer,src,len,maxlen) {\
	if(videobuffer.length() + (len) > (size_t)maxlen) return -1;\
	char* pBuftmp = (char*)videobuffer.c_str();\
	if(pBuftmp == NULL) assert(0);\
	memcpy(pBuftmp + videobuffer.length(), src, len);\
	videobuffer.resize(videobuffer.length() + (len));\
}

#define SAFE_SET_H264_STARTCODE(videobuffer,maxlen) {\
	if (videobuffer.length() + H264_STATRTCODE_LEN > (size_t)maxlen) return -1;\
	char* pBuftmp = (char*)videobuffer.c_str();\
	if(pBuftmp == NULL) assert(0);\
	memcpy(pBuftmp + videobuffer.length(),H264_STATRTCODE_ADDR,H264_STATRTCODE_LEN);\
	videobuffer.resize(videobuffer.length() + H264_STATRTCODE_LEN);\
}






RtpH264Analyzer::RtpH264Analyzer(const CBFreamCallBack& callback, const std::string& pSps, const std::string& pPps)
{
	m_pFramCallBack = callback;

	memset(&m_stFuIndictor, 0, sizeof(FU_INDICATOR));
	memset(&m_stNalHeader,	0, sizeof(FU_HEADER));

	m_nFreamType			= FrameType_Video;

	m_bFirstSeq				= -2;
	m_nLastSeq				= -2;

	m_bThrowPack			= false;
	m_bIsRecvingIDRFream	= false;
	m_bIsWaitingIDRFream	= false;
	
		
	m_pSpsBuffer			= pSps;
	m_pPpsBuffer			= pPps;
}

RtpH264Analyzer::~RtpH264Analyzer()
{

}

int RtpH264Analyzer::InputData(const shared_ptr<STREAM_TRANS_INFO>& transinfo, const shared_ptr<RTPPackage>& rtp)
{
	const RTPHEADER& m_stRtpHeader = rtp->rtpHeader();
	


	//����Ƿ��ж���
	int nCurrentSeq = ntohs(m_stRtpHeader.seq);

	//��һ��������ʱ�򲻽����ж϶���,��ʱ�� m_nLastSeq Ϊ��ʼ״̬,���㶪��
	if (m_nLastSeq + 1 < nCurrentSeq && m_nLastSeq != m_bFirstSeq)
	{
		m_bThrowPack   = true;
		if (true == m_bIsRecvingIDRFream)
		{
			m_bIsWaitingIDRFream = true;
		}
	}
	//65535��ʾseqѭ�������һ��,��һ��ʱΪ0,�������������������,�������л���
	else if (m_nLastSeq + 1 > nCurrentSeq && m_nLastSeq != 65535)
	{
		//������
	}

	const char* pBuf = rtp->rtpDataAddr();
	uint32_t nBufLen = rtp->rtpDataLen();
	uint32_t extBufLen = rtp->rtpExternLen();

	if (m_frame == NULL) m_frame = make_shared<RTPFrame>();
	m_frame->pushRTPPackage(rtp);

	if (extBufLen > 0)
	{
		m_frame->extendData(String(pBuf, extBufLen));
	
		pBuf += extBufLen;
		nBufLen -= extBufLen;
	}
	m_nLastSeq = nCurrentSeq;
	if (nBufLen <= 0)
	{
		if (m_stRtpHeader.m && m_frame)
		{
			m_pFramCallBack(m_frame);

			m_frame = NULL;
		}

		return 0;
	}

	//get nalu header
	memcpy(&m_stFuIndictor, pBuf, 1);

	/*RFC3984 5.2  RTP���ظ�ʽ�Ĺ����ṹ
	  ע��:���淶û�����Ʒ�װ�ڵ���NAL��Ԫ���ͷ�Ƭ���Ĵ�С����װ�ھۺϰ��е�NAL��Ԫ��СΪ65535�ֽ�
	  ����NAL��Ԫ��: ������ֻ����һ��NAL��Ԫ��NALͷ���������ԭʼNAL��Ԫ����,���ڷ�Χ1~23֮��
	*/
	if ( m_stFuIndictor.TYPE >0 &&  m_stFuIndictor.TYPE < 24)
	{
		String videobuffer;
		if (m_stFuIndictor.TYPE != 5)
		{
			int nVideoBufferSize = H264_STATRTCODE_LEN + nBufLen;
			videobuffer.alloc(nVideoBufferSize + 100);

			//������NAL������,��������֡������ʼ��
			SAFE_SET_H264_STARTCODE(videobuffer, nVideoBufferSize);

			SAFE_COPY_VIDEO(videobuffer, pBuf, nBufLen, nVideoBufferSize);
		}
		else
		{
			int nVideoBufferSize = H264_STATRTCODE_LEN + (int)m_pSpsBuffer.length() + (int)m_pPpsBuffer.length() + nBufLen;
			videobuffer.alloc(nVideoBufferSize + 100);

			SAFE_COPY_VIDEO(videobuffer, m_pSpsBuffer.c_str(), m_pSpsBuffer.length(), nVideoBufferSize);
			SAFE_COPY_VIDEO(videobuffer, m_pPpsBuffer.c_str(), m_pPpsBuffer.length(), nVideoBufferSize);

			SAFE_SET_H264_STARTCODE(videobuffer, nVideoBufferSize);

			SAFE_COPY_VIDEO(videobuffer, pBuf, nBufLen, nVideoBufferSize);
		}
		m_nFreamType = FrameType_Video_PFrame;
		switch (m_stFuIndictor.TYPE)
		{
		case 0://δ����
			break;
		case 1://��IDR֡ͼ�񲻲������ݻ���Ƭ��
			m_nFreamType = FrameType_Video_PFrame;
//			m_pFramCallBack(m_pVideoBuf, m_nVideoBufLen, m_nFreamType, ntohl(m_stRtpHeader.ts), (uint64_t)m_dwUser, m_lpUser);
			break;
		case 2://��IDR֡ͼ����A�����ݻ���Ƭ��
			break;
		case 3://��IDR֡ͼ����B�����ݻ���Ƭ��
			break;
		case 4://��IDR֡ͼ����C�����ݻ���Ƭ��
			break;
		case 5://IDR֡��ͼ��Ƭ��
			m_nFreamType = FrameType_Video_IFrame;
//			m_pFramCallBack(m_pVideoBuf, m_nVideoBufLen, m_nFreamType, ntohl(m_stRtpHeader.ts), (uint64_t)m_dwUser, m_lpUser);
			break;
		case 6://������ǿ��Ϣ(SEI)
			break;
		case 7://���в�����(SPS)
			m_nFreamType = FrameType_Video_IFrame;
			m_pSpsBuffer = videobuffer;
//			m_pFramCallBack(m_pVideoBuf, m_nVideoBufLen, m_nFreamType, ntohl(m_stRtpHeader.ts), (uint64_t)m_dwUser, m_lpUser);
			break;
		case 8://ͼ�������(PPS)
			m_nFreamType = FrameType_Video_IFrame;
			m_pPpsBuffer = videobuffer;
//			m_pFramCallBack(m_pVideoBuf, m_nVideoBufLen, m_nFreamType, ntohl(m_stRtpHeader.ts), (uint64_t)m_dwUser, m_lpUser);
			break;
		case 9://�ָ���
			break;
		case 10://���н�����
			break;
		case 11://��������
			break;
		case 12://�������
			break;
		case 13://���в�������չ
			break;
		case 14://����
			break;
		case 15://����
			break;
		case 16://����
			break;
		case 17://����
			break;
		case 18://����
			break;
		case 19://δ�ָ�ĸ�������ͼ��Ƭ��
			break;
		case 20://����
			break;
		case 21://����
			break;
		case 22://����
			break;
		case 23://����
			break;
		default://δ����
			break;
		}
		if (videobuffer.length() == 0)
		{
			return 0;
		}
		/*shared_ptr<RTPAnalyzer_FrameInfo> frame = make_shared<RTPAnalyzer_FrameInfo>();
		frame->codeid = CodeID_Video_H264;
		frame->frametype = m_nFreamType;
		frame->frame = videobuffer;
		frame->timestmap = ntohl(m_stRtpHeader.ts);

		m_pFramCallBack(frame);*/

		return 0;
	}
	/*RFC3984 5.2  RTP���ظ�ʽ�Ĺ����ṹ
	  �ۺϰ�: ���������ھۺ϶��NAL��Ԫ������RTP�����С�������4�ְ汾,��ʱ��ۺϰ�����A(STAP-A)
	  ��ʱ��ۺϰ�����B(STAP-B),��ʱ��ۺϰ�����(MTAP)16λλ��(MTAP16), ��ʱ��ۺϰ�����(MTAP)24λλ��(MTAP24)
	  ���� STAP-A, STAP-B, MTAP16, MTAP24��NAL��Ԫ���ͺŷֱ���24, 25, 26, 27��
	*/
	else if (24 == m_stFuIndictor.TYPE)//STAP-A
	{
	}
	else if (25 == m_stFuIndictor.TYPE)//STAP-B
	{
	}
	else if (26 == m_stFuIndictor.TYPE)//MTAP16
	{
	}
	else if (27 == m_stFuIndictor.TYPE)//MTAP24
	{
	}
	/*RFC3984 5.2  RTP���ظ�ʽ�Ĺ����ṹ
	  ��Ƭ��: ���ڷ�Ƭ����NAL��Ԫ�����RTP��, �ִ��������汾FU-A, FU-B, ��NAL��Ԫ����28, 29��ʶ��
	*/
	else if (28 == m_stFuIndictor.TYPE)//FU_A
	{
		//if (m_pVideoBuf.length() <= 0) m_pVideoBuf.alloc(VIEOD_FRAME_LEN + 100);

		int NALType = pBuf[1] & 0x1f;
		memset(&m_stNalHeader, 0 ,sizeof(m_stNalHeader));
		memcpy(&m_stNalHeader, pBuf + 1, 1);//FU_HEADER��ֵ
		//�յ�֡��Ƭ���ĵ�һ������
		if (1 == m_stNalHeader.S) 
		{
			//�����ʱΪ����״̬,��ôֹͣ����
			m_bThrowPack = false;
			//���IDR֡
			//5 == m_stFuHeader.TYPE ��׼�жϷ���
			//7 == m_stFuHeader.TYPE Ϊ�˼�������SNP3370
			m_nFreamType = FrameType_Video_PFrame;
			if (5 == m_stNalHeader.TYPE || 7 == m_stNalHeader.TYPE)
			{
				m_nFreamType = FrameType_Video_IFrame;
				m_bIsWaitingIDRFream = false;
				m_bIsRecvingIDRFream = true; //���ڽ���IDR֡

				//IDR֡ǰд��sps��pps
				m_frame->pushBuffer(m_pSpsBuffer);
				m_frame->pushBuffer(m_pPpsBuffer);
				//SAFE_COPY_VIDEO(m_pVideoBuf, m_pSpsBuffer.c_str(), m_pSpsBuffer.length(), VIEOD_FRAME_LEN);
				//SAFE_COPY_VIDEO(m_pVideoBuf, m_pPpsBuffer.c_str(), m_pPpsBuffer.length(), VIEOD_FRAME_LEN);
			}

			//����֡���ݵ���ʼ��
			m_frame->pushBuffer(H264_STATRTCODE_ADDR, H264_STATRTCODE_LEN);
			//SAFE_SET_H264_STARTCODE(m_pVideoBuf, VIEOD_FRAME_LEN);
						
			char pBufer1 = ( pBuf[0] & 0xE0 ) | NALType ;
			m_frame->pushBuffer(&pBufer1, 1);
			//SAFE_COPY_VIDEO(m_pVideoBuf, &pBufer1, 1, VIEOD_FRAME_LEN);

			//������������,����ǰ���IDR֡����λ��FU_HEADER�����ֽ�
			m_frame->pushRTPBuffer(pBuf + 2, nBufLen - 2);
			//SAFE_COPY_VIDEO(m_pVideoBuf, pBuf + 2, nBufLen - 2, VIEOD_FRAME_LEN);
		}
		//֡��Ƭ�������һ������,Ϊ�˸�ǿ�ļ����Էſ�����
		//������ΪFU_HEADER��E�ֶκ�rtspͷ��markλ������һ������Ч��
		else if (1 == m_stRtpHeader.m || 1 == m_stNalHeader.E)
		{
			if (true == m_bThrowPack || true == m_bIsWaitingIDRFream)
			{
				m_frame = NULL;
				return -1;
			}
			m_frame->pushRTPBuffer(pBuf + 2, nBufLen - 2);
			m_frame->codeId(CodeID_Video_H264);
			m_frame->frameType(m_nFreamType);
			m_frame->timestmap(ntohl(m_stRtpHeader.ts));

//			SAFE_COPY_VIDEO(m_pVideoBuf, pBuf + 2, nBufLen - 2, VIEOD_FRAME_LEN);

			//shared_ptr<RTPAnalyzer_FrameInfo> frame = make_shared<RTPAnalyzer_FrameInfo>();
			//frame->codeid = CodeID_Video_H264;
			//frame->frametype = m_nFreamType;
			//frame->frame= m_pVideoBuf;
			//frame->timestmap = ntohl(m_stRtpHeader.ts);

			m_pFramCallBack(m_frame);

			m_frame = NULL;

			return 0;
		}
		else
		{
			//�������֡�ĵ�һ������,���Ҵ��ڶ�֡״̬,��ô�յ������а�������
			if (true == m_bThrowPack || true == m_bIsWaitingIDRFream) 
			{
				m_frame = NULL;
				return -1;
			}

			//�������,��������
			m_frame->pushRTPBuffer(pBuf + 2, nBufLen - 2);
			//SAFE_COPY_VIDEO(m_pVideoBuf, pBuf + 2, nBufLen - 2, VIEOD_FRAME_LEN);
		}
	}
	else if (29 == m_stFuIndictor.TYPE)//FU-B
	{
	}
	else//30-31 δ����
	{
	}

	return 0;
}


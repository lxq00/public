#ifndef __ONVIFPROTOCOL_H__GetNode
#define __ONVIFPROTOCOL_H__GetNode
#include "CmdObject.h"


class CmdGetNode :public CmdObject
{
public:
	CmdGetNode(const std::string& _token) :token(_token)
	{
		action = "http://www.onvif.org/ver20/ptz/wsdl/GetNode";
	}
	virtual ~CmdGetNode() {}

	virtual std::string build(const URL& URL)
	{
		stringstream stream;

		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
			<< "<s:Envelope " << onvif_xml_ns << ">"
			<< buildHeader(URL)
			<< "<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
			<< "<GetNode xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">"
			<< "<NodeToken>"<<token<<"</NodeToken>"
			<< "</GetNode>"
			<< "</s:Body></s:Envelope>";

		return stream.str();
	}
	shared_ptr<OnvifClientDefs::_PTZConfig> ptzcfg;
	virtual bool parse(XMLN * p_xml)
	{
		ptzcfg = make_shared<OnvifClientDefs::_PTZConfig>();

		XMLN * p_res = xml_node_soap_get(p_xml, "GetNodeResponse");
		if (NULL == p_res)
		{
			return false;
		}

		XMLN * PTZNode = xml_node_soap_get(p_res, "PTZNode");
		if (PTZNode)
		{
			XMLN * p_Space = xml_node_soap_get(PTZNode, "SupportedPTZSpaces");
			if (p_Space)
			{
				XMLN * p_AbsolutePanTiltPositionSpace = xml_node_soap_get(p_Space, "AbsolutePanTiltPositionSpace");
				if (p_AbsolutePanTiltPositionSpace)
				{
					XMLN * p_XRange = xml_node_soap_get(p_AbsolutePanTiltPositionSpace, "XRange");
					if (p_XRange)
					{
						XMLN * p_XMin = xml_node_soap_get(p_XRange, "Min");
						if (p_XMin)
						{
							ptzcfg->pantilt_x.min = (float)atof(p_XMin->data);
						}
						XMLN * p_XMax = xml_node_soap_get(p_XRange, "Max");
						if (p_XMax)
						{
							ptzcfg->pantilt_x.max = (float)atof(p_XMax->data);
						}
					}

					XMLN * p_YRange = xml_node_soap_get(p_AbsolutePanTiltPositionSpace, "YRange");
					if (p_YRange)
					{
						XMLN * p_XMin = xml_node_soap_get(p_YRange, "Min");
						if (p_XMin)
						{
							ptzcfg->pantilt_y.min = (float)atof(p_XMin->data);
						}
						XMLN * p_XMax = xml_node_soap_get(p_YRange, "Max");
						if (p_XMax)
						{
							ptzcfg->pantilt_y.max = (float)atof(p_XMax->data);
						}
					}
				}
				XMLN * p_AbsoluteZoomPositionSpace = xml_node_soap_get(p_Space, "AbsoluteZoomPositionSpace");
				if (p_AbsoluteZoomPositionSpace)
				{
					XMLN * p_XRange = xml_node_soap_get(p_AbsoluteZoomPositionSpace, "XRange");
					if (p_XRange)
					{
						XMLN * p_XMin = xml_node_soap_get(p_XRange, "Min");
						if (p_XMin)
						{
							ptzcfg->zoom.min = (float)atof(p_XMin->data);
						}
						XMLN * p_XMax = xml_node_soap_get(p_XRange, "Max");
						if (p_XMax)
						{
							ptzcfg->zoom.max = (float)atof(p_XMax->data);
						}
					}
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;

		}

		return true;
	}

private:
	std::string token;
};



#endif //__ONVIFPROTOCOL_H__

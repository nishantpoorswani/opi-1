//============================================================
// Description:
//
// This file is for RTL8192E Co-exist mechanism
//
// History
// 2012/11/15 Cosa first check in.
//
//============================================================

//============================================================
// include files
//============================================================
#include "Mp_Precomp.h"

#if WPP_SOFTWARE_TRACE
#include "HalBtc8192e1Ant.tmh"
#endif

#if(BT_30_SUPPORT == 1)
//============================================================
// Global variables, these are static variables
//============================================================
static COEX_DM_8192E_1ANT		GLCoexDm8192e1Ant;
static PCOEX_DM_8192E_1ANT 	pCoexDm=&GLCoexDm8192e1Ant;
static COEX_STA_8192E_1ANT		GLCoexSta8192e1Ant;
static PCOEX_STA_8192E_1ANT	pCoexSta=&GLCoexSta8192e1Ant;

const char *const GLBtInfoSrc8192e1Ant[]={
	"BT Info[wifi fw]",
	"BT Info[bt rsp]",
	"BT Info[bt auto report]",
};

u4Byte	GLCoexVerDate8192e1Ant=20140527;
u4Byte	GLCoexVer8192e1Ant=0x4f;

//============================================================
// local function proto type if needed
//============================================================
//============================================================
// local function start with halbtc8192e1ant_
//============================================================
u1Byte
halbtc8192e1ant_BtRssiState(
	u1Byte			levelNum,
	u1Byte			rssiThresh,
	u1Byte			rssiThresh1
	)
{
	s4Byte			btRssi=0;
	u1Byte			btRssiState=pCoexSta->preBtRssiState;

	btRssi = pCoexSta->btRssi;

	if(levelNum == 2)
	{
		if( (pCoexSta->preBtRssiState == BTC_RSSI_STATE_LOW) ||
			(pCoexSta->preBtRssiState == BTC_RSSI_STATE_STAY_LOW))
		{
			if(btRssi >= (rssiThresh+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				btRssiState = BTC_RSSI_STATE_HIGH;
			}
			else
			{
				btRssiState = BTC_RSSI_STATE_STAY_LOW;
			}
		}
		else
		{
			if(btRssi < rssiThresh)
			{
				btRssiState = BTC_RSSI_STATE_LOW;
			}
			else
			{
				btRssiState = BTC_RSSI_STATE_STAY_HIGH;
			}
		}
	}
	else if(levelNum == 3)
	{
		if(rssiThresh > rssiThresh1)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Rssi thresh error!!\n"));
			return pCoexSta->preBtRssiState;
		}

		if( (pCoexSta->preBtRssiState == BTC_RSSI_STATE_LOW) ||
			(pCoexSta->preBtRssiState == BTC_RSSI_STATE_STAY_LOW))
		{
			if(btRssi >= (rssiThresh+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				btRssiState = BTC_RSSI_STATE_MEDIUM;
			}
			else
			{
				btRssiState = BTC_RSSI_STATE_STAY_LOW;
			}
		}
		else if( (pCoexSta->preBtRssiState == BTC_RSSI_STATE_MEDIUM) ||
			(pCoexSta->preBtRssiState == BTC_RSSI_STATE_STAY_MEDIUM))
		{
			if(btRssi >= (rssiThresh1+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				btRssiState = BTC_RSSI_STATE_HIGH;
			}
			else if(btRssi < rssiThresh)
			{
				btRssiState = BTC_RSSI_STATE_LOW;
			}
			else
			{
				btRssiState = BTC_RSSI_STATE_STAY_MEDIUM;
			}
		}
		else
		{
			if(btRssi < rssiThresh1)
			{
				btRssiState = BTC_RSSI_STATE_MEDIUM;
			}
			else
			{
				btRssiState = BTC_RSSI_STATE_STAY_HIGH;
			}
		}
	}

	pCoexSta->preBtRssiState = btRssiState;

	return btRssiState;
}

u1Byte
halbtc8192e1ant_WifiRssiState(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			index,
	IN	u1Byte			levelNum,
	IN	u1Byte			rssiThresh,
	IN	u1Byte			rssiThresh1
	)
{
	s4Byte			wifiRssi=0;
	u1Byte			wifiRssiState=pCoexSta->preWifiRssiState[index];

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_S4_WIFI_RSSI, &wifiRssi);

	if(levelNum == 2)
	{
		if( (pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_LOW) ||
			(pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_STAY_LOW))
		{
			if(wifiRssi >= (rssiThresh+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				wifiRssiState = BTC_RSSI_STATE_HIGH;
			}
			else
			{
				wifiRssiState = BTC_RSSI_STATE_STAY_LOW;
			}
		}
		else
		{
			if(wifiRssi < rssiThresh)
			{
				wifiRssiState = BTC_RSSI_STATE_LOW;
			}
			else
			{
				wifiRssiState = BTC_RSSI_STATE_STAY_HIGH;
			}
		}
	}
	else if(levelNum == 3)
	{
		if(rssiThresh > rssiThresh1)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], wifi RSSI thresh error!!\n"));
			return pCoexSta->preWifiRssiState[index];
		}

		if( (pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_LOW) ||
			(pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_STAY_LOW))
		{
			if(wifiRssi >= (rssiThresh+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				wifiRssiState = BTC_RSSI_STATE_MEDIUM;
			}
			else
			{
				wifiRssiState = BTC_RSSI_STATE_STAY_LOW;
			}
		}
		else if( (pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_MEDIUM) ||
			(pCoexSta->preWifiRssiState[index] == BTC_RSSI_STATE_STAY_MEDIUM))
		{
			if(wifiRssi >= (rssiThresh1+BTC_RSSI_COEX_THRESH_TOL_8192E_1ANT))
			{
				wifiRssiState = BTC_RSSI_STATE_HIGH;
			}
			else if(wifiRssi < rssiThresh)
			{
				wifiRssiState = BTC_RSSI_STATE_LOW;
			}
			else
			{
				wifiRssiState = BTC_RSSI_STATE_STAY_MEDIUM;
			}
		}
		else
		{
			if(wifiRssi < rssiThresh1)
			{
				wifiRssiState = BTC_RSSI_STATE_MEDIUM;
			}
			else
			{
				wifiRssiState = BTC_RSSI_STATE_STAY_HIGH;
			}
		}
	}

	pCoexSta->preWifiRssiState[index] = wifiRssiState;

	return wifiRssiState;
}

VOID
halbtc8192e1ant_UpdateRaMask(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u4Byte				disRateMask
	)
{
	pCoexDm->curRaMask = disRateMask;

	if( bForceExec || (pCoexDm->preRaMask != pCoexDm->curRaMask))
	{
		pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_UPDATE_RAMASK, &pCoexDm->curRaMask);
	}
	pCoexDm->preRaMask = pCoexDm->curRaMask;
}

VOID
halbtc8192e1ant_AutoRateFallbackRetry(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u1Byte				type
	)
{
	BOOLEAN	bWifiUnderBMode=FALSE;

	pCoexDm->curArfrType = type;

	if( bForceExec || (pCoexDm->preArfrType != pCoexDm->curArfrType))
	{
		switch(pCoexDm->curArfrType)
		{
			case 0:	// normal mode
				pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x430, pCoexDm->backupArfrCnt1);
				pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x434, pCoexDm->backupArfrCnt2);
				break;
			case 1:
				pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_UNDER_B_MODE, &bWifiUnderBMode);
				if(bWifiUnderBMode)
				{
					pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x430, 0x0);
					pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x434, 0x01010101);
				}
				else
				{
					pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x430, 0x0);
					pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x434, 0x04030201);
				}
				break;
			default:
				break;
		}
	}

	pCoexDm->preArfrType = pCoexDm->curArfrType;
}

VOID
halbtc8192e1ant_RetryLimit(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u1Byte				type
	)
{
	pCoexDm->curRetryLimitType = type;

	if( bForceExec || (pCoexDm->preRetryLimitType != pCoexDm->curRetryLimitType))
	{
		switch(pCoexDm->curRetryLimitType)
		{
			case 0:	// normal mode
				pBtCoexist->fBtcWrite2Byte(pBtCoexist, 0x42a, pCoexDm->backupRetryLimit);
				break;
			case 1:	// retry limit=8
				pBtCoexist->fBtcWrite2Byte(pBtCoexist, 0x42a, 0x0808);
				break;
			default:
				break;
		}
	}

	pCoexDm->preRetryLimitType = pCoexDm->curRetryLimitType;
}

VOID
halbtc8192e1ant_AmpduMaxTime(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u1Byte				type
	)
{
	pCoexDm->curAmpduTimeType = type;

	if( bForceExec || (pCoexDm->preAmpduTimeType != pCoexDm->curAmpduTimeType))
	{
		switch(pCoexDm->curAmpduTimeType)
		{
			case 0:	// normal mode
				pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x456, pCoexDm->backupAmpduMaxTime);
				break;
			case 1:	// AMPDU timw = 0x38 * 32us
				pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x456, 0x38);
				break;
			default:
				break;
		}
	}

	pCoexDm->preAmpduTimeType = pCoexDm->curAmpduTimeType;
}

VOID
halbtc8192e1ant_LimitedTx(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u1Byte				raMaskType,
	IN	u1Byte				arfrType,
	IN	u1Byte				retryLimitType,
	IN	u1Byte				ampduTimeType
	)
{
	switch(raMaskType)
	{
		case 0:	// normal mode
			halbtc8192e1ant_UpdateRaMask(pBtCoexist, bForceExec, 0x0);
			break;
		case 1:	// disable cck 1/2
			halbtc8192e1ant_UpdateRaMask(pBtCoexist, bForceExec, 0x00000003);
			break;
		case 2:	// disable cck 1/2/5.5, ofdm 6/9/12/18/24, mcs 0/1/2/3/4
			halbtc8192e1ant_UpdateRaMask(pBtCoexist, bForceExec, 0x0001f1f7);
			break;
		default:
			break;
	}

	halbtc8192e1ant_AutoRateFallbackRetry(pBtCoexist, bForceExec, arfrType);
	halbtc8192e1ant_RetryLimit(pBtCoexist, bForceExec, retryLimitType);
	halbtc8192e1ant_AmpduMaxTime(pBtCoexist, bForceExec, ampduTimeType);
}

VOID
halbtc8192e1ant_LimitedRx(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	BOOLEAN				bRejApAggPkt,
	IN	BOOLEAN				bBtCtrlAggBufSize,
	IN	u1Byte				aggBufSize
	)
{
	BOOLEAN	bRejectRxAgg=bRejApAggPkt;
	BOOLEAN	bBtCtrlRxAggSize=bBtCtrlAggBufSize;
	u1Byte	rxAggSize=aggBufSize;

	//============================================
	//	Rx Aggregation related setting
	//============================================
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_TO_REJ_AP_AGG_PKT, &bRejectRxAgg);
	// decide BT control aggregation buf size or not
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_CTRL_AGG_SIZE, &bBtCtrlRxAggSize);
	// aggregation buf size, only work when BT control Rx aggregation size.
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_U1_AGG_BUF_SIZE, &rxAggSize);
	// real update aggregation setting
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_AGGREGATE_CTRL, NULL);


}

VOID
halbtc8192e1ant_QueryBtInfo(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	u1Byte			H2C_Parameter[1] ={0};

	pCoexSta->bC2hBtInfoReqSent = TRUE;

	H2C_Parameter[0] |= BIT0;	// trigger

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], Query Bt Info, FW write 0x61=0x%x\n",
		H2C_Parameter[0]));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x61, 1, H2C_Parameter);
}

VOID
halbtc8192e1ant_MonitorBtCtr(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	u4Byte 			regHPTxRx, regLPTxRx, u4Tmp, u4Tmp1;
	u4Byte			regHPTx=0, regHPRx=0, regLPTx=0, regLPRx=0;
	u1Byte			u1Tmp, u1Tmp1;
	s4Byte			wifiRssi;
	static u1Byte		NumOfBtCounterChk = 0;

       //to avoid 0x76e[3] = 1 (WLAN_Act control by PTA) during IPS
	//if (! (pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x76e) & 0x8) )

	if (pCoexSta->bUnderIps)
	{
		pCoexSta->highPriorityTx = 65535;
		pCoexSta->highPriorityRx = 65535;
		pCoexSta->lowPriorityTx = 65535;
		pCoexSta->lowPriorityRx = 65535;
		return;
	}

	regHPTxRx = 0x770;
	regLPTxRx = 0x774;

	u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, regHPTxRx);
	regHPTx = u4Tmp & bMaskLWord;
	regHPRx = (u4Tmp & bMaskHWord)>>16;

	u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, regLPTxRx);
	regLPTx = u4Tmp & bMaskLWord;
	regLPRx = (u4Tmp & bMaskHWord)>>16;

	pCoexSta->highPriorityTx = regHPTx;
	pCoexSta->highPriorityRx = regHPRx;
	pCoexSta->lowPriorityTx = regLPTx;
	pCoexSta->lowPriorityRx = regLPRx;

	if( (pCoexSta->lowPriorityTx >= 1050)  && (!pCoexSta->bC2hBtInquiryPage))
		pCoexSta->popEventCnt++;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Hi-Pri Rx/Tx: %d/%d, Lo-Pri Rx/Tx: %d/%d\n",
		regHPRx, regHPTx, regLPRx, regLPTx));

	// reset counter
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x76e, 0xc);

	if ((regHPTx == 0) && (regHPRx ==0) && (regLPTx == 0) && (regLPRx == 0))
	{
		NumOfBtCounterChk++;
		if (NumOfBtCounterChk >= 3)
		{
			halbtc8192e1ant_QueryBtInfo(pBtCoexist);
			NumOfBtCounterChk = 0;
		}
	}
}


VOID
halbtc8192e1ant_MonitorWiFiCtr(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	u4Byte 	u4Tmp;
	u2Byte 	u2Tmp[3];
	s4Byte	wifiRssi=0;
	BOOLEAN bWifiBusy = FALSE, bWifiUnderBMode = FALSE;
	static u1Byte nCCKLockCounter = 0;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_S4_WIFI_RSSI, &wifiRssi);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_UNDER_B_MODE, &bWifiUnderBMode);

	if (pCoexSta->bUnderIps)
	{
		pCoexSta->nCRCOK_CCK = 0;
		pCoexSta->nCRCOK_11g = 0;
		pCoexSta->nCRCOK_11n = 0;
		pCoexSta->nCRCOK_11nAgg = 0;

		pCoexSta->nCRCErr_CCK = 0;
		pCoexSta->nCRCErr_11g = 0;
		pCoexSta->nCRCErr_11n = 0;
		pCoexSta->nCRCErr_11nAgg = 0;
	}
	else
	{
		pCoexSta->nCRCOK_CCK	= pBtCoexist->fBtcRead4Byte(pBtCoexist, 0xf88);
		pCoexSta->nCRCOK_11g 	= pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xf94);
		pCoexSta->nCRCOK_11n	= pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xf90);
		pCoexSta->nCRCOK_11nAgg= pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xfb8);

		pCoexSta->nCRCErr_CCK 	 = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0xf84);
		pCoexSta->nCRCErr_11g 	 = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xf96);
		pCoexSta->nCRCErr_11n 	 = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xf92);
		pCoexSta->nCRCErr_11nAgg = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0xfba);
	}


	//reset counter
	pBtCoexist->fBtcWrite1ByteBitMask(pBtCoexist, 0xf16, 0x1, 0x1);
	pBtCoexist->fBtcWrite1ByteBitMask(pBtCoexist, 0xf16, 0x1, 0x0);

	if ( (bWifiBusy) && (wifiRssi >= 30) && (!bWifiUnderBMode))
	{
		if ( (pCoexDm->btStatus == BT_8192E_1ANT_BT_STATUS_ACL_BUSY) ||
			(pCoexDm->btStatus == BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY) ||
			(pCoexDm->btStatus == BT_8192E_1ANT_BT_STATUS_SCO_BUSY) )
		{
			if (pCoexSta->nCRCOK_CCK >(pCoexSta->nCRCOK_11g + pCoexSta->nCRCOK_11n +
				pCoexSta->nCRCOK_11nAgg) )
			{
				if (nCCKLockCounter < 5)
				 nCCKLockCounter++;
			}
			else
			{
				if (nCCKLockCounter > 0)
				 nCCKLockCounter--;
			}

		}
		else
		{
			if (nCCKLockCounter > 0)
			  nCCKLockCounter--;
		}
	}
	else
	{
		if (nCCKLockCounter > 0)
			nCCKLockCounter--;
	}

	if (!pCoexSta->bPreCCKLock)
	{

		if (nCCKLockCounter >= 5)
		 pCoexSta->bCCKLock = TRUE;
		else
		 pCoexSta->bCCKLock = FALSE;
	}
	else
	{
		if (nCCKLockCounter == 0)
		 pCoexSta->bCCKLock = FALSE;
		else
		 pCoexSta->bCCKLock = TRUE;
	}

	pCoexSta->bPreCCKLock =  pCoexSta->bCCKLock;


}

BOOLEAN
halbtc8192e1ant_IsWifiStatusChanged(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	static BOOLEAN	bPreWifiBusy=FALSE, bPreUnder4way=FALSE, bPreBtHsOn=FALSE;
	BOOLEAN bWifiBusy=FALSE, bUnder4way=FALSE, bBtHsOn=FALSE;
	BOOLEAN bWifiConnected=FALSE;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_4_WAY_PROGRESS, &bUnder4way);

	if(bWifiConnected)
	{
		if(bWifiBusy != bPreWifiBusy)
		{
			bPreWifiBusy = bWifiBusy;
			return TRUE;
		}
		if(bUnder4way != bPreUnder4way)
		{
			bPreUnder4way = bUnder4way;
			return TRUE;
		}
		if(bBtHsOn != bPreBtHsOn)
		{
			bPreBtHsOn = bBtHsOn;
			return TRUE;
		}
	}

	return FALSE;
}

VOID
halbtc8192e1ant_UpdateBtLinkInfo(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO	pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN				bBtHsOn=FALSE;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);

	pBtLinkInfo->bBtLinkExist = pCoexSta->bBtLinkExist;
	pBtLinkInfo->bScoExist = pCoexSta->bScoExist;
	pBtLinkInfo->bA2dpExist = pCoexSta->bA2dpExist;
	pBtLinkInfo->bPanExist = pCoexSta->bPanExist;
	pBtLinkInfo->bHidExist = pCoexSta->bHidExist;

	// work around for HS mode.
	if(bBtHsOn)
	{
		pBtLinkInfo->bPanExist = TRUE;
		pBtLinkInfo->bBtLinkExist = TRUE;
	}

	// check if Sco only
	if( pBtLinkInfo->bScoExist &&
		!pBtLinkInfo->bA2dpExist &&
		!pBtLinkInfo->bPanExist &&
		!pBtLinkInfo->bHidExist )
		pBtLinkInfo->bScoOnly = TRUE;
	else
		pBtLinkInfo->bScoOnly = FALSE;

	// check if A2dp only
	if( !pBtLinkInfo->bScoExist &&
		pBtLinkInfo->bA2dpExist &&
		!pBtLinkInfo->bPanExist &&
		!pBtLinkInfo->bHidExist )
		pBtLinkInfo->bA2dpOnly = TRUE;
	else
		pBtLinkInfo->bA2dpOnly = FALSE;

	// check if Pan only
	if( !pBtLinkInfo->bScoExist &&
		!pBtLinkInfo->bA2dpExist &&
		pBtLinkInfo->bPanExist &&
		!pBtLinkInfo->bHidExist )
		pBtLinkInfo->bPanOnly = TRUE;
	else
		pBtLinkInfo->bPanOnly = FALSE;

	// check if Hid only
	if( !pBtLinkInfo->bScoExist &&
		!pBtLinkInfo->bA2dpExist &&
		!pBtLinkInfo->bPanExist &&
		pBtLinkInfo->bHidExist )
		pBtLinkInfo->bHidOnly = TRUE;
	else
		pBtLinkInfo->bHidOnly = FALSE;
}

u1Byte
halbtc8192e1ant_ActionAlgorithm(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO	pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN				bBtHsOn=FALSE;
	u1Byte				algorithm=BT_8192E_1ANT_COEX_ALGO_UNDEFINED;
	u1Byte				numOfDiffProfile=0;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);

	if(!pBtLinkInfo->bBtLinkExist)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], No BT link exists!!!\n"));
		return algorithm;
	}

	if(pBtLinkInfo->bScoExist)
		numOfDiffProfile++;
	if(pBtLinkInfo->bHidExist)
		numOfDiffProfile++;
	if(pBtLinkInfo->bPanExist)
		numOfDiffProfile++;
	if(pBtLinkInfo->bA2dpExist)
		numOfDiffProfile++;

	if(numOfDiffProfile == 1)
	{
		if(pBtLinkInfo->bScoExist)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO only\n"));
			algorithm = BT_8192E_1ANT_COEX_ALGO_SCO;
		}
		else
		{
			if(pBtLinkInfo->bHidExist)
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID only\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_HID;
			}
			else if(pBtLinkInfo->bA2dpExist)
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = A2DP only\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_A2DP;
			}
			else if(pBtLinkInfo->bPanExist)
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = PAN(HS) only\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANHS;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = PAN(EDR) only\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR;
				}
			}
		}
	}
	else if(numOfDiffProfile == 2)
	{
		if(pBtLinkInfo->bScoExist)
		{
			if(pBtLinkInfo->bHidExist)
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + HID\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_HID;
			}
			else if(pBtLinkInfo->bA2dpExist)
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + A2DP ==> SCO\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_SCO;
			}
			else if(pBtLinkInfo->bPanExist)
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_SCO;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + PAN(EDR)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_HID;
				}
			}
		}
		else
		{
			if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bA2dpExist )
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID + A2DP\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_HID_A2DP;
			}
			else if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bPanExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_HID_A2DP;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID + PAN(EDR)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_HID;
				}
			}
			else if( pBtLinkInfo->bPanExist &&
				pBtLinkInfo->bA2dpExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = A2DP + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_A2DP_PANHS;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = A2DP + PAN(EDR)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_A2DP;
				}
			}
		}
	}
	else if(numOfDiffProfile == 3)
	{
		if(pBtLinkInfo->bScoExist)
		{
			if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bA2dpExist )
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + HID + A2DP ==> HID\n"));
				algorithm = BT_8192E_1ANT_COEX_ALGO_HID;
			}
			else if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bPanExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + HID + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_HID_A2DP;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + HID + PAN(EDR)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_HID;
				}
			}
			else if( pBtLinkInfo->bPanExist &&
				pBtLinkInfo->bA2dpExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + A2DP + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_SCO;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + A2DP + PAN(EDR) ==> HID\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_HID;
				}
			}
		}
		else
		{
			if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bPanExist &&
				pBtLinkInfo->bA2dpExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID + A2DP + PAN(HS)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_HID_A2DP;
				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = HID + A2DP + PAN(EDR)\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_HID_A2DP_PANEDR;
				}
			}
		}
	}
	else if(numOfDiffProfile >= 3)
	{
		if(pBtLinkInfo->bScoExist)
		{
			if( pBtLinkInfo->bHidExist &&
				pBtLinkInfo->bPanExist &&
				pBtLinkInfo->bA2dpExist )
			{
				if(bBtHsOn)
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Error!!! BT Profile = SCO + HID + A2DP + PAN(HS)\n"));

				}
				else
				{
					RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT Profile = SCO + HID + A2DP + PAN(EDR)==>PAN(EDR)+HID\n"));
					algorithm = BT_8192E_1ANT_COEX_ALGO_PANEDR_HID;
				}
			}
		}
	}

	return algorithm;
}

VOID
halbtc8192e1ant_SetBtAutoReport(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bEnableAutoReport
	)
{
	u1Byte			H2C_Parameter[1] ={0};

	H2C_Parameter[0] = 0;

	if(bEnableAutoReport)
	{
		H2C_Parameter[0] |= BIT0;
	}

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], BT FW auto report : %s, FW write 0x68=0x%x\n",
		(bEnableAutoReport? "Enabled!!":"Disabled!!"), H2C_Parameter[0]));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x68, 1, H2C_Parameter);
}

VOID
halbtc8192e1ant_BtAutoReport(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	BOOLEAN			bEnableAutoReport
	)
{
	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], %s BT Auto report = %s\n",
		(bForceExec? "force to":""), ((bEnableAutoReport)? "Enabled":"Disabled")));
	pCoexDm->bCurBtAutoReport = bEnableAutoReport;

	if(!bForceExec)
	{
		if(pCoexDm->bPreBtAutoReport == pCoexDm->bCurBtAutoReport)
			return;
	}
	halbtc8192e1ant_SetBtAutoReport(pBtCoexist, pCoexDm->bCurBtAutoReport);

	pCoexDm->bPreBtAutoReport = pCoexDm->bCurBtAutoReport;
}

VOID
halbtc8192e1ant_SetSwPenaltyTxRateAdaptive(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bLowPenaltyRa
	)
{
	u1Byte			H2C_Parameter[6] ={0};

	H2C_Parameter[0] = 0x6;	// opCode, 0x6= Retry_Penalty

	if(bLowPenaltyRa)
	{
		H2C_Parameter[1] |= BIT0;
		H2C_Parameter[2] = 0x00;  //normal rate except MCS7/6/5, OFDM54/48/36
		H2C_Parameter[3] = 0xf7;  //MCS7 or OFDM54
		H2C_Parameter[4] = 0xf8;  //MCS6 or OFDM48
		H2C_Parameter[5] = 0xf9;	//MCS5 or OFDM36
	}

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set WiFi Low-Penalty Retry: %s",
		(bLowPenaltyRa? "ON!!":"OFF!!") ));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x69, 6, H2C_Parameter);
}

VOID
halbtc8192e1ant_LowPenaltyRa(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	BOOLEAN			bLowPenaltyRa
	)
{
	pCoexDm->bCurLowPenaltyRa = bLowPenaltyRa;

	if(!bForceExec)
	{
		if(pCoexDm->bPreLowPenaltyRa == pCoexDm->bCurLowPenaltyRa)
			return;
	}
	halbtc8192e1ant_SetSwPenaltyTxRateAdaptive(pBtCoexist, pCoexDm->bCurLowPenaltyRa);

	pCoexDm->bPreLowPenaltyRa = pCoexDm->bCurLowPenaltyRa;
}

VOID
halbtc8192e1ant_SetCoexTable(
	IN	PBTC_COEXIST	pBtCoexist,
	IN	u4Byte		val0x6c0,
	IN	u4Byte		val0x6c4,
	IN	u4Byte		val0x6c8,
	IN	u1Byte		val0x6cc
	)
{
	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set coex table, set 0x6c0=0x%x\n", val0x6c0));
	pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x6c0, val0x6c0);

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set coex table, set 0x6c4=0x%x\n", val0x6c4));
	pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x6c4, val0x6c4);

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set coex table, set 0x6c8=0x%x\n", val0x6c8));
	pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x6c8, val0x6c8);

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set coex table, set 0x6cc=0x%x\n", val0x6cc));
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cc, val0x6cc);
}

VOID
halbtc8192e1ant_CoexTable(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	u4Byte			val0x6c0,
	IN	u4Byte			val0x6c4,
	IN	u4Byte			val0x6c8,
	IN	u1Byte			val0x6cc
	)
{
	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], %s write Coex Table 0x6c0=0x%x, 0x6c4=0x%x, 0x6cc=0x%x\n",
		(bForceExec? "force to":""), val0x6c0, val0x6c4, val0x6cc));
	pCoexDm->curVal0x6c0 = val0x6c0;
	pCoexDm->curVal0x6c4 = val0x6c4;
	pCoexDm->curVal0x6c8 = val0x6c8;
	pCoexDm->curVal0x6cc = val0x6cc;

	if(!bForceExec)
	{
		if( (pCoexDm->preVal0x6c0 == pCoexDm->curVal0x6c0) &&
			(pCoexDm->preVal0x6c4 == pCoexDm->curVal0x6c4) &&
			(pCoexDm->preVal0x6c8 == pCoexDm->curVal0x6c8) &&
			(pCoexDm->preVal0x6cc == pCoexDm->curVal0x6cc) )
			return;
	}
	halbtc8192e1ant_SetCoexTable(pBtCoexist, val0x6c0, val0x6c4, val0x6c8, val0x6cc);

	pCoexDm->preVal0x6c0 = pCoexDm->curVal0x6c0;
	pCoexDm->preVal0x6c4 = pCoexDm->curVal0x6c4;
	pCoexDm->preVal0x6c8 = pCoexDm->curVal0x6c8;
	pCoexDm->preVal0x6cc = pCoexDm->curVal0x6cc;
}

VOID
halbtc8192e1ant_CoexTableWithType(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bForceExec,
	IN	u1Byte				type
	)
{
	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ********** CoexTable(%d) **********\n", type));

	pCoexSta->nCoexTableType = type;

	switch(type)
	{
		case 0:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x55555555, 0x55555555, 0xffffff, 0x3);
			break;
		case 1:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x55555555, 0x5a5a5a5a, 0xffffff, 0x3);
			break;
		case 2:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x5a5a5a5a, 0x5a5a5a5a, 0xffffff, 0x3);
			break;
		case 3:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x55555555, 0x5a5a5a5a, 0xffffff, 0x3);
			break;
		case 4:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x55555555, 0xaaaa5a5a, 0xffffff, 0x3);
			break;
		case 5:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x5a5a5a5a, 0xaa5a5a5a, 0xffffff, 0x3);
			break;
		case 6:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0x55555555, 0xaaaaaaaa, 0xffffff, 0x3);
			break;
		case 7:
			halbtc8192e1ant_CoexTable(pBtCoexist, bForceExec, 0xaaaaaaaa, 0xaaaaaaaa, 0xffffff, 0x3);
			break;
		default:
			break;
	}
}

VOID
halbtc8192e1ant_SetFwIgnoreWlanAct(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bEnable
	)
{
	u1Byte			H2C_Parameter[1] ={0};

	if(bEnable)
	{
		H2C_Parameter[0] |= BIT0;		// function enable
	}

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], set FW for BT Ignore Wlan_Act, FW write 0x63=0x%x\n",
		H2C_Parameter[0]));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x63, 1, H2C_Parameter);
}

VOID
halbtc8192e1ant_IgnoreWlanAct(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	BOOLEAN			bEnable
	)
{
	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], %s turn Ignore WlanAct %s\n",
		(bForceExec? "force to":""), (bEnable? "ON":"OFF")));
	pCoexDm->bCurIgnoreWlanAct = bEnable;

	if(!bForceExec)
	{
		if(pCoexDm->bPreIgnoreWlanAct == pCoexDm->bCurIgnoreWlanAct)
			return;
	}
	halbtc8192e1ant_SetFwIgnoreWlanAct(pBtCoexist, bEnable);

	pCoexDm->bPreIgnoreWlanAct = pCoexDm->bCurIgnoreWlanAct;
}

VOID
halbtc8192e1ant_SetLpsRpwm(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			lpsVal,
	IN	u1Byte			rpwmVal
	)
{
	u1Byte	lps=lpsVal;
	u1Byte	rpwm=rpwmVal;

	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_U1_LPS_VAL, &lps);
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_U1_RPWM_VAL, &rpwm);
}

VOID
halbtc8192e1ant_LpsRpwm(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	u1Byte			lpsVal,
	IN	u1Byte			rpwmVal
	)
{
	BOOLEAN	bForceExecPwrCmd=FALSE;

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], %s set lps/rpwm=0x%x/0x%x \n",
		(bForceExec? "force to":""), lpsVal, rpwmVal));
	pCoexDm->curLps = lpsVal;
	pCoexDm->curRpwm = rpwmVal;

	if(!bForceExec)
	{
		if( (pCoexDm->preLps == pCoexDm->curLps) &&
			(pCoexDm->preRpwm == pCoexDm->curRpwm) )
		{
			return;
		}
	}
	halbtc8192e1ant_SetLpsRpwm(pBtCoexist, lpsVal, rpwmVal);

	pCoexDm->preLps = pCoexDm->curLps;
	pCoexDm->preRpwm = pCoexDm->curRpwm;
}

VOID
halbtc8192e1ant_SwMechanism(
	IN	PBTC_COEXIST	pBtCoexist,
	IN	BOOLEAN 		bLowPenaltyRA
	)
{
	halbtc8192e1ant_LowPenaltyRa(pBtCoexist, NORMAL_EXEC, bLowPenaltyRA);
}

VOID
halbtc8192e1ant_SetAntPath(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte				antPosType,
	IN	BOOLEAN				bInitHwCfg,
	IN	BOOLEAN				bWifiOff
	)
{
	u4Byte			u4Tmp=0;

	if(bInitHwCfg)
	{
		pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x944, 0x24);
		pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x930, 0x700700);
		if(pBtCoexist->chipInterface == BTC_INTF_USB)
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x64, 0x30430004);
		else
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x64, 0x30030004);

		// 0x4c[27][24]='00', Set Antenna to BB
		u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x4c);
		u4Tmp &= ~BIT24;
		u4Tmp &= ~BIT27;
		pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x4c, u4Tmp);
	}
	else if(bWifiOff)
	{
		if(pBtCoexist->chipInterface == BTC_INTF_USB)
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x64, 0x30430004);
		else
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x64, 0x30030004);

		// 0x4c[27][24]='11', Set Antenna to BT, 0x64[8:7]=0, 0x64[2]=1
		u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x4c);
		u4Tmp |= BIT24;
		u4Tmp |= BIT27;
		pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x4c, u4Tmp);
	}

	// ext switch setting
	switch(antPosType)
	{
		case BTC_ANT_PATH_WIFI:
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x92c, 0x4);
			break;
		case BTC_ANT_PATH_BT:
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x92c, 0x20);
			break;
		default:
		case BTC_ANT_PATH_PTA:
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x92c, 0x4);
			break;
	}
}

VOID
halbtc8192e1ant_SetFwPstdma(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			byte1,
	IN	u1Byte			byte2,
	IN	u1Byte			byte3,
	IN	u1Byte			byte4,
	IN	u1Byte			byte5
	)
{
	u1Byte			H2C_Parameter[5] ={0};
	u1Byte			realByte1=byte1, realByte5=byte5;
	BOOLEAN			bApEnable=FALSE;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_AP_MODE_ENABLE, &bApEnable);

	if(bApEnable)
	{
		if(byte1&BIT4 && !(byte1&BIT5))
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], FW for 1Ant AP mode\n"));
			realByte1 &= ~BIT4;
			realByte1 |= BIT5;

			realByte5 |= BIT5;
			realByte5 &= ~BIT6;
		}
	}

	H2C_Parameter[0] = realByte1;
	H2C_Parameter[1] = byte2;
	H2C_Parameter[2] = byte3;
	H2C_Parameter[3] = byte4;
	H2C_Parameter[4] = realByte5;

	pCoexDm->psTdmaPara[0] = realByte1;
	pCoexDm->psTdmaPara[1] = byte2;
	pCoexDm->psTdmaPara[2] = byte3;
	pCoexDm->psTdmaPara[3] = byte4;
	pCoexDm->psTdmaPara[4] = realByte5;

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], PS-TDMA H2C cmd =0x%x%08x\n",
		H2C_Parameter[0],
		H2C_Parameter[1]<<24|H2C_Parameter[2]<<16|H2C_Parameter[3]<<8|H2C_Parameter[4]));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x60, 5, H2C_Parameter);
}


VOID
halbtc8192e1ant_PsTdma(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bForceExec,
	IN	BOOLEAN			bTurnOn,
	IN	u1Byte			type
	)
{
	PBTC_BOARD_INFO	pBoardInfo=&pBtCoexist->boardInfo;
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN			bTurnOnByCnt=FALSE, bWifiBusy=FALSE, bWiFiNoisy=FALSE;
	u1Byte			psTdmaTypeByCnt=0, rssiAdjustVal=0;
	u1Byte			psTdmaByte4Val = 0x50, psTdmaByte0Val = 0x51, psTdmaByte3Val =  0x10;
	s1Byte			nWiFiDurationAdjust = 0x0;

	pCoexDm->bCurPsTdmaOn = bTurnOn;
	pCoexDm->curPsTdma = type;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);

	if (pCoexDm->bCurPsTdmaOn)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ********** TDMA(on, %d) **********\n",
			pCoexDm->curPsTdma));
	}
	else
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ********** TDMA(off, %d) **********\n",
			pCoexDm->curPsTdma));
	}

	if(!bForceExec)
	{
		if( (pCoexDm->bPrePsTdmaOn == pCoexDm->bCurPsTdmaOn) &&
			(pCoexDm->prePsTdma == pCoexDm->curPsTdma) )
			return;
	}

	if (pCoexSta->nScanAPNum <= 5)
	 	nWiFiDurationAdjust = 5;
	else if  (pCoexSta->nScanAPNum >= 40)
	 	nWiFiDurationAdjust = -15;
	else if  (pCoexSta->nScanAPNum >= 20)
	 	nWiFiDurationAdjust = -10;

	if (!pCoexSta->bForceLpsOn)  //only for A2DP-only case 1/2/9/11 while wifi noisy threshold > 30
	{
		psTdmaByte0Val = 0x61;  //no null-pkt
		psTdmaByte3Val = 0x11; // no tx-pause at BT-slot
		psTdmaByte4Val = 0x10; // 0x778 = d/1 toggle
	}

	if (  (type == 3) || (type == 13) || (type == 14) )
		psTdmaByte4Val = psTdmaByte4Val & 0xbf;  //no dynamic slot for multi-profile

	if (pBtLinkInfo->bSlaveRole == TRUE)
		psTdmaByte4Val = psTdmaByte4Val | 0x1;  //0x778 = 0x1 at wifi slot (no blocking BT Low-Pri pkts)

	if(bTurnOn)
	{
		switch(type)
		{
			default:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x1a, 0x1a, 0x0, psTdmaByte4Val);
				break;
			case 1:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, psTdmaByte0Val, 0x3a+nWiFiDurationAdjust, 0x03, psTdmaByte3Val, psTdmaByte4Val);
				break;
			case 2:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, psTdmaByte0Val, 0x2d+nWiFiDurationAdjust, 0x03, psTdmaByte3Val, psTdmaByte4Val);
				break;
			case 3:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x1d, 0x1d, 0x0, psTdmaByte4Val);
				break;
			case 4:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x93, 0x15, 0x3, 0x14, 0x0);
				break;
			case 5:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x15, 0x3, 0x11, 0x11);
				break;
			case 6:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x20, 0x3, 0x11, 0x11);
				break;
			case 7:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x13, 0xc, 0x5, 0x0, 0x0);
				break;
			case 8:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x93, 0x25, 0x3, 0x10, 0x0);
				break;
			case 9:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, psTdmaByte0Val, 0x21, 0x3, psTdmaByte3Val, psTdmaByte4Val);
				break;
			case 10:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x13, 0xa, 0xa, 0x0, 0x40);
				break;
			case 11:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, psTdmaByte0Val, 0x21, 0x03, psTdmaByte3Val, psTdmaByte4Val);
				break;
			case 12:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x0a, 0x0a, 0x0, 0x50);
				break;
			case 13:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x12, 0x12, 0x0, psTdmaByte4Val);
				break;
			case 14:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x21, 0x3, 0x10, psTdmaByte4Val);
				break;
			case 15:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x13, 0xa, 0x3, 0x8, 0x0);
				break;
			case 16:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x93, 0x15, 0x3, 0x10, 0x0);
				break;
			case 18:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x93, 0x25, 0x3, 0x10, 0x0);
				break;
			case 20:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x3f, 0x03, 0x11, 0x10);
				break;
			case 21:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x25, 0x03, 0x11, 0x11);
				break;
			case 22:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x25, 0x03, 0x11, 0x10);
				break;
			case 23:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xe3, 0x25, 0x3, 0x31, 0x18);
				break;
			case 24:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xe3, 0x15, 0x3, 0x31, 0x18);
				break;
			case 25:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xe3, 0xa, 0x3, 0x31, 0x18);
				break;
			case 26:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xe3, 0xa, 0x3, 0x31, 0x18);
				break;
			case 27:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xe3, 0x25, 0x3, 0x31, 0x98);
				break;
			case 28:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x69, 0x25, 0x3, 0x31, 0x0);
				break;
			case 29:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xab, 0x1a, 0x1a, 0x1, 0x10);
				break;
			case 30:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x51, 0x30, 0x3, 0x10, 0x10);
				break;
			case 31:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xd3, 0x1a, 0x1a, 0, 0x58);
				break;
			case 32:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x61, 0x35, 0x3, 0x11, 0x11);
				break;
			case 33:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xa3, 0x25, 0x3, 0x30, 0x90);
				break;
			case 34:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x53, 0x1a, 0x1a, 0x0, 0x10);
				break;
			case 35:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x63, 0x1a, 0x1a, 0x0, 0x10);
				break;
			case 36:
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0xd3, 0x12, 0x3, 0x14, 0x50);
				break;
			case 40: // SoftAP only with no sta associated,BT disable ,TDMA mode for power saving
				/* here softap mode screen off will cost 70-80mA for phone */
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x23, 0x18, 0x00, 0x10, 0x24);
				break;
		}
	}
	else
	{

		// disable PS tdma
		switch(type)
		{
			case 8: //PTA Control
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x8, 0x0, 0x0, 0x0, 0x0);
				halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_PTA, FALSE, FALSE);
				break;
			case 0:
			default:  //Software control, Antenna at BT side
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x0, 0x0, 0x0, 0x0, 0x0);
				halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, FALSE, FALSE);
				break;
			case 9:   //Software control, Antenna at WiFi side
				halbtc8192e1ant_SetFwPstdma(pBtCoexist, 0x0, 0x0, 0x0, 0x0, 0x0);
				halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_WIFI, FALSE, FALSE);
				break;
		}
	}
	rssiAdjustVal =0;
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_U1_RSSI_ADJ_VAL_FOR_1ANT_COEX_TYPE, &rssiAdjustVal);


	RT_TRACE(COMP_COEX, DBG_LOUD, ("############# [BTCoex], 0x948=0x%x, 0x765=0x%x, 0x67=0x%x\n",
		pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x948),   pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x765), pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x67)));

	// update pre state
	pCoexDm->bPrePsTdmaOn = pCoexDm->bCurPsTdmaOn;
	pCoexDm->prePsTdma = pCoexDm->curPsTdma;
}

VOID
halbtc8192e1ant_CoexAllOff(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	// sw all off
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

	// hw all off
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 0);
}

BOOLEAN
halbtc8192e1ant_IsCommonAction(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	BOOLEAN			bCommon=FALSE, bWifiConnected=FALSE, bWifiBusy=FALSE;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);

	if(!bWifiConnected &&
		BT_8192E_1ANT_BT_STATUS_NON_CONNECTED_IDLE == pCoexDm->btStatus)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi non connected-idle + BT non connected-idle!!\n"));

 		//halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

		bCommon = TRUE;
	}
	else if(bWifiConnected &&
		(BT_8192E_1ANT_BT_STATUS_NON_CONNECTED_IDLE == pCoexDm->btStatus) )
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi connected + BT non connected-idle!!\n"));

		//halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

		bCommon = TRUE;
	}
	else if(!bWifiConnected &&
		(BT_8192E_1ANT_BT_STATUS_CONNECTED_IDLE == pCoexDm->btStatus) )
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi non connected-idle + BT connected-idle!!\n"));

		//halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

		bCommon = TRUE;
	}
	else if(bWifiConnected &&
		(BT_8192E_1ANT_BT_STATUS_CONNECTED_IDLE == pCoexDm->btStatus) )
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi connected + BT connected-idle!!\n"));

		//halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

		bCommon = TRUE;
	}
	else if(!bWifiConnected &&
		(BT_8192E_1ANT_BT_STATUS_CONNECTED_IDLE != pCoexDm->btStatus) )
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi non connected-idle + BT Busy!!\n"));

		//halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

		bCommon = TRUE;
	}
	else
	{
		if (bWifiBusy)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi Connected-Busy + BT Busy!!\n"));
		}
		else
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Wifi Connected-Idle + BT Busy!!\n"));
		}

		bCommon = FALSE;
	}

	return bCommon;
}


VOID
halbtc8192e1ant_TdmaDurationAdjustForAcl(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte				wifiStatus
	)
{
	static s4Byte		up,dn,m,n,WaitCount;
	s4Byte			result;   //0: no change, +1: increase WiFi duration, -1: decrease WiFi duration
	u1Byte			retryCount=0, btInfoExt;
	static BOOLEAN	bPreWifiBusy=FALSE;
	BOOLEAN			bWifiBusy = FALSE;

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], TdmaDurationAdjustForAcl()\n"));

	if(BT_8192E_1ANT_WIFI_STATUS_CONNECTED_BUSY == wifiStatus)
		bWifiBusy = TRUE;
	else
		bWifiBusy = FALSE;

	if( (BT_8192E_1ANT_WIFI_STATUS_NON_CONNECTED_ASSO_AUTH_SCAN == wifiStatus) ||
		(BT_8192E_1ANT_WIFI_STATUS_CONNECTED_SCAN == wifiStatus) ||
		(BT_8192E_1ANT_WIFI_STATUS_CONNECTED_SPECIAL_PKT == wifiStatus) )
	{
		if( pCoexDm->curPsTdma != 1 &&
			pCoexDm->curPsTdma != 2 &&
			pCoexDm->curPsTdma != 3 &&
			pCoexDm->curPsTdma != 9 )
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 9);
			pCoexDm->psTdmaDuAdjType = 9;

			up = 0;
			dn = 0;
			m = 1;
			n= 3;
			result = 0;
			WaitCount = 0;
		}
		return;
	}

	if(!pCoexDm->bAutoTdmaAdjust)
	{
		pCoexDm->bAutoTdmaAdjust = TRUE;
		RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], first run TdmaDurationAdjust()!!\n"));

		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 2);
		pCoexDm->psTdmaDuAdjType = 2;
		//============
		up = 0;
		dn = 0;
		m = 1;
		n= 3;
		result = 0;
		WaitCount = 0;
	}
	else
	{
		//accquire the BT TRx retry count from BT_Info byte2
		retryCount = pCoexSta->btRetryCnt;
		btInfoExt = pCoexSta->btInfoExt;

		if ( (pCoexSta->lowPriorityTx) > 1050 ||  (pCoexSta->lowPriorityRx) > 1250 )
			retryCount++;

		result = 0;
		WaitCount++;

		if(retryCount == 0)  // no retry in the last 2-second duration
		{
			up++;
			dn--;

			if (dn <= 0)
				dn = 0;

			if(up >= n)	// if 連續 n個2秒 retry count為0, 則調寬WiFi duration
			{
				WaitCount = 0;
				n = 3;
				up = 0;
				dn = 0;
				result = 1;
				RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], Increase wifi duration!!\n"));
			}
		}
		else if (retryCount <= 3)	// <=3 retry in the last 2-second duration
		{
			up--;
			dn++;

			if (up <= 0)
				up = 0;

			if (dn == 2)	// if 連續 2個2秒 retry count< 3, 則調窄WiFi duration
			{
				if (WaitCount <= 2)
					m++; // 避免一直在兩個level中來回
				else
					m = 1;

				if ( m >= 20) //m 最大值 = 20 ' 最大120秒 recheck是否調整 WiFi duration.
					m = 20;

				n = 3*m;
				up = 0;
				dn = 0;
				WaitCount = 0;
				result = -1;
				RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], Decrease wifi duration for retryCounter<3!!\n"));
			}
		}
		else  //retry count > 3, 只要1次 retry count > 3, 則調窄WiFi duration
		{
			if (WaitCount == 1)
				m++; // 避免一直在兩個level中來回
			else
				m = 1;

			if ( m >= 20) //m 最大值 = 20 ' 最大120秒 recheck是否調整 WiFi duration.
				m = 20;

			n = 3*m;
			up = 0;
			dn = 0;
			WaitCount = 0;
			result = -1;
			RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], Decrease wifi duration for retryCounter>3!!\n"));
		}

		if(result == -1)
		{
			if( (BT_INFO_8192E_1ANT_A2DP_BASIC_RATE(btInfoExt)) &&
				((pCoexDm->curPsTdma == 1) ||(pCoexDm->curPsTdma == 2)) )
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 9);
				pCoexDm->psTdmaDuAdjType = 9;
			}
			else if(pCoexDm->curPsTdma == 1)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 2);
				pCoexDm->psTdmaDuAdjType = 2;
			}
			else if(pCoexDm->curPsTdma == 2)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 9);
				pCoexDm->psTdmaDuAdjType = 9;
			}
			else if(pCoexDm->curPsTdma == 9)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 11);
				pCoexDm->psTdmaDuAdjType = 11;
			}
		}
		else if(result == 1)
		{
			if( (BT_INFO_8192E_1ANT_A2DP_BASIC_RATE(btInfoExt)) &&
				((pCoexDm->curPsTdma == 1) ||(pCoexDm->curPsTdma == 2)) )
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 9);
				pCoexDm->psTdmaDuAdjType = 9;
			}
			else if(pCoexDm->curPsTdma == 11)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 9);
				pCoexDm->psTdmaDuAdjType = 9;
			}
			else if(pCoexDm->curPsTdma == 9)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 2);
				pCoexDm->psTdmaDuAdjType = 2;
			}
			else if(pCoexDm->curPsTdma == 2)
			{
				halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 1);
				pCoexDm->psTdmaDuAdjType = 1;
			}
		}
		else	  //no change
		{
			/* Bryant Modify
			if(bWifiBusy != bPreWifiBusy)  //if busy / idle change
			{
				bPreWifiBusy = bWifiBusy;
				halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, TRUE, pCoexDm->curPsTdma);
			}
			*/

			RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], ********** TDMA(on, %d) **********\n",
				pCoexDm->curPsTdma));
		}

		if( pCoexDm->curPsTdma != 1 &&
			pCoexDm->curPsTdma != 2 &&
			pCoexDm->curPsTdma != 9 &&
			pCoexDm->curPsTdma != 11 )
		{
			// recover to previous adjust type
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, pCoexDm->psTdmaDuAdjType);
		}
	}
}

VOID
halbtc8192e1ant_PsTdmaCheckForPowerSaveState(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN			bNewPsState
	)
{
	u1Byte	lpsMode=0x0;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U1_LPS_MODE, &lpsMode);

	if(lpsMode)	// already under LPS state
	{
		if(bNewPsState)
		{
			// keep state under LPS, do nothing.
		}
		else
		{
			// will leave LPS state, turn off psTdma first
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 0);
		}
	}
	else						// NO PS state
	{
		if(bNewPsState)
		{
			// will enter LPS state, turn off psTdma first
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 0);
		}
		else
		{
			// keep state under NO PS state, do nothing.
		}
	}
}

VOID
halbtc8192e1ant_PowerSaveState(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte				psType,
	IN	u1Byte				lpsVal,
	IN	u1Byte				rpwmVal
	)
{
	BOOLEAN		bLowPwrDisable=FALSE;

	switch(psType)
	{
		case BTC_PS_WIFI_NATIVE:
			// recover to original 32k low power setting
			bLowPwrDisable = FALSE;
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_DISABLE_LOW_POWER, &bLowPwrDisable);
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_NORMAL_LPS, NULL);
			pCoexSta->bForceLpsOn = FALSE;
			break;
		case BTC_PS_LPS_ON:
			halbtc8192e1ant_PsTdmaCheckForPowerSaveState(pBtCoexist, TRUE);
			halbtc8192e1ant_LpsRpwm(pBtCoexist, NORMAL_EXEC, lpsVal, rpwmVal);
			// when coex force to enter LPS, do not enter 32k low power.
			bLowPwrDisable = TRUE;
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_DISABLE_LOW_POWER, &bLowPwrDisable);
			// power save must executed before psTdma.
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_ENTER_LPS, NULL);
			pCoexSta->bForceLpsOn = TRUE;
			break;
		case BTC_PS_LPS_OFF:
			halbtc8192e1ant_PsTdmaCheckForPowerSaveState(pBtCoexist, FALSE);
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_LEAVE_LPS, NULL);
			pCoexSta->bForceLpsOn = FALSE;
			break;
		default:
			break;
	}
}

VOID
halbtc8192e1ant_ActionWifiOnly(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 0);
	halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 9);
}

VOID
halbtc8192e1ant_MonitorBtEnableDisable(
	IN 	PBTC_COEXIST		pBtCoexist
	)
{
	static BOOLEAN	bPreBtDisabled=FALSE;
	static u4Byte		btDisableCnt=0;
	BOOLEAN			bBtActive=TRUE, bBtDisabled=FALSE;

	// This function check if bt is disabled

	if(	pCoexSta->highPriorityTx == 0 &&
		pCoexSta->highPriorityRx == 0 &&
		pCoexSta->lowPriorityTx == 0 &&
		pCoexSta->lowPriorityRx == 0)
	{
		bBtActive = FALSE;
	}
	if(	pCoexSta->highPriorityTx == 0xffff &&
		pCoexSta->highPriorityRx == 0xffff &&
		pCoexSta->lowPriorityTx == 0xffff &&
		pCoexSta->lowPriorityRx == 0xffff)
	{
		bBtActive = FALSE;
	}
	if(bBtActive)
	{
		btDisableCnt = 0;
		bBtDisabled = FALSE;
		pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_DISABLE, &bBtDisabled);
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT is enabled !!\n"));
	}
	else
	{
		btDisableCnt++;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], bt all counters=0, %d times!!\n",
				btDisableCnt));
		if(btDisableCnt >= 2)
		{
			bBtDisabled = TRUE;
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_DISABLE, &bBtDisabled);
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT is disabled !!\n"));
			halbtc8192e1ant_ActionWifiOnly(pBtCoexist);
		}
	}
	if(bPreBtDisabled != bBtDisabled)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT is from %s to %s!!\n",
			(bPreBtDisabled ? "disabled":"enabled"),
			(bBtDisabled ? "disabled":"enabled")));
		bPreBtDisabled = bBtDisabled;
		if(!bBtDisabled)
		{
		}
		else
		{
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_LEAVE_LPS, NULL);
			pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_ACT_NORMAL_LPS, NULL);
		}
	}
}

//=============================================
//
//	Software Coex Mechanism start
//
//=============================================

// SCO only or SCO+PAN(HS)

/*
VOID
halbtc8192e1ant_ActionSco(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
}


VOID
halbtc8192e1ant_ActionHid(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
}

//A2DP only / PAN(EDR) only/ A2DP+PAN(HS)
VOID
halbtc8192e1ant_ActionA2dp(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
}

VOID
halbtc8192e1ant_ActionA2dpPanHs(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
}

VOID
halbtc8192e1ant_ActionPanEdr(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
}

//PAN(HS) only
VOID
halbtc8192e1ant_ActionPanHs(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
}

//PAN(EDR)+A2DP
VOID
halbtc8192e1ant_ActionPanEdrA2dp(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
}

VOID
halbtc8192e1ant_ActionPanEdrHid(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
}

// HID+A2DP+PAN(EDR)
VOID
halbtc8192e1ant_ActionHidA2dpPanEdr(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
}

VOID
halbtc8192e1ant_ActionHidA2dp(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
}

*/

//=============================================
//
//	Non-Software Coex Mechanism start
//
//=============================================
VOID
halbtc8192e1ant_ActionWifiMultiPort(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
}

VOID
halbtc8192e1ant_ActionHs(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 5);
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
}

VOID
halbtc8192e1ant_ActionBtInquiry(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN			bWifiConnected=FALSE, bApEnable=FALSE, bWifiBusy=FALSE, bBtBusy=FALSE;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_AP_MODE_ENABLE, &bApEnable);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_TRAFFIC_BUSY, &bBtBusy);

	if((!bWifiConnected) && (!pCoexSta->bWiFiIsHighPriTask))
	{
		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);

	 	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 0);
	}
	else if((pBtLinkInfo->bScoExist) || (pBtLinkInfo->bHidExist) || (pBtLinkInfo->bA2dpExist))
	{
		// SCO/HID/A2DP  busy
		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);

		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else if ((pBtLinkInfo->bPanExist) || (bWifiBusy))
	{
		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 20);

		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else
	{
		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);

		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 7);
	}
}

VOID
halbtc8192e1ant_ActionBtScoHidOnlyBusy(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte				wifiStatus
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN	bWifiConnected=FALSE;
	u1Byte	wifiRssiState=BTC_RSSI_STATE_HIGH;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);

	// tdma and coex table

	if(pBtLinkInfo->bScoExist)
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 5);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 5);
	}
	else //HID
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 6);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 5);
	}
}

VOID
halbtc8192e1ant_ActionWifiConnectedBtAclBusy(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte				wifiStatus
	)
{
	u1Byte		btRssiState;

	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;
	btRssiState = halbtc8192e1ant_BtRssiState(2, 28, 0);

	if ( (pCoexSta->lowPriorityRx >= 1000)  && (pCoexSta->lowPriorityRx != 65535) )
	{
		pBtLinkInfo->bSlaveRole = TRUE;
	}
	else
	{
		pBtLinkInfo->bSlaveRole = FALSE;
	}

	if(pBtLinkInfo->bHidOnly)  //HID
	{
		halbtc8192e1ant_ActionBtScoHidOnlyBusy(pBtCoexist, wifiStatus);
		pCoexDm->bAutoTdmaAdjust = FALSE;
		return;
	}
	else if(pBtLinkInfo->bA2dpOnly)  //A2DP
	{
		if(BT_8192E_1ANT_WIFI_STATUS_CONNECTED_IDLE == wifiStatus)
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
			pCoexDm->bAutoTdmaAdjust = FALSE;
		}
		else
		{
			halbtc8192e1ant_TdmaDurationAdjustForAcl(pBtCoexist, wifiStatus);
#if 0
			if (pCoexSta->bCCKLock)
				halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 3);
			else
#endif
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
			pCoexDm->bAutoTdmaAdjust = TRUE;
		}
	}
	else if ( ((pBtLinkInfo->bA2dpExist) && (pBtLinkInfo->bPanExist)) ||
		       (pBtLinkInfo->bHidExist&&pBtLinkInfo->bA2dpExist&&pBtLinkInfo->bPanExist) ) //A2DP+PAN(OPP,FTP), HID+A2DP+PAN(OPP,FTP)
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 13);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		pCoexDm->bAutoTdmaAdjust = FALSE;
	}
	else if(pBtLinkInfo->bHidExist&&pBtLinkInfo->bA2dpExist)  //HID+A2DP
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 14);
		pCoexDm->bAutoTdmaAdjust = FALSE;

		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 3);
	}
	else if( (pBtLinkInfo->bPanOnly) || (pBtLinkInfo->bHidExist&&pBtLinkInfo->bPanExist) ) //PAN(OPP,FTP), HID+PAN(OPP,FTP)
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 3);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		pCoexDm->bAutoTdmaAdjust = FALSE;
	}
	else
	{
		//BT no-profile busy (0x9)
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		pCoexDm->bAutoTdmaAdjust = FALSE;
	}
}

VOID
halbtc8192e1ant_ActionWifiNotConnected(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	// power save state
	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, FALSE, 8);
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 0);
}

VOID
halbtc8192e1ant_ActionWifiNotConnectedScan(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;

	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	if(BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus)
	{
		if (pBtLinkInfo->bA2dpExist)
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		}
		else if (pBtLinkInfo->bA2dpExist && pBtLinkInfo->bPanExist)
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 22);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	 	}
	 	else
	 	{
		        halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 20);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		}
	}
	else if( (BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
			(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
	{
		halbtc8192e1ant_ActionBtScoHidOnlyBusy(pBtCoexist,
			BT_8192E_1ANT_WIFI_STATUS_CONNECTED_SCAN);
	}
	else
	{
		//Bryant Add
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
	}
}

VOID
halbtc8192e1ant_ActionWifiNotConnectedAssoAuth(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;

	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	if( (pBtLinkInfo->bScoExist)  || (pBtLinkInfo->bHidExist) ||  (pBtLinkInfo->bA2dpExist) )
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else if (pBtLinkInfo->bPanExist)
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 20);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
	}
}

VOID
halbtc8192e1ant_ActionWifiConnectedScan(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;

	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	if(BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus)
	{
		if (pBtLinkInfo->bA2dpExist)
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
		}
		else if (pBtLinkInfo->bA2dpExist && pBtLinkInfo->bPanExist)
	 	{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 22);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	 	}
	 	else
	 	{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 20);
			halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	 	}
	}
	else if( (BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
			(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
	{
		halbtc8192e1ant_ActionBtScoHidOnlyBusy(pBtCoexist,
			BT_8192E_1ANT_WIFI_STATUS_CONNECTED_SCAN);
	}
	else
	{
		//Bryant Add
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
	}
}

VOID
halbtc8192e1ant_ActionWifiConnectedSpecialPacket(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;

	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	if((pBtLinkInfo->bScoExist) || (pBtLinkInfo->bHidExist) || (pBtLinkInfo->bA2dpExist))
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 32);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else if(pBtLinkInfo->bPanExist)
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, TRUE, 20);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 4);
	}
	else
	{
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
	}
}

VOID
halbtc8192e1ant_ActionWifiConnected(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	BOOLEAN 	bWifiBusy=FALSE;
	BOOLEAN		bScan=FALSE, bLink=FALSE, bRoam=FALSE;
	BOOLEAN		bUnder4way=FALSE, bApEnable=FALSE;
	u4Byte		wifiBw;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CoexForWifiConnect()===>\n"));

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_4_WAY_PROGRESS, &bUnder4way);
	if(bUnder4way)
	{
		halbtc8192e1ant_ActionWifiConnectedSpecialPacket(pBtCoexist);
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CoexForWifiConnect(), return for wifi is under 4way<===\n"));
		return;
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_SCAN, &bScan);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_LINK, &bLink);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_ROAM, &bRoam);
	if(bScan || bLink || bRoam)
	{
		if(bScan)
			halbtc8192e1ant_ActionWifiConnectedScan(pBtCoexist);
		else
			halbtc8192e1ant_ActionWifiConnectedSpecialPacket(pBtCoexist);
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CoexForWifiConnect(), return for wifi is under scan<===\n"));
		return;
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_AP_MODE_ENABLE, &bApEnable);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_BUSY, &bWifiBusy);

	// power save state
	if(!bApEnable && BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus && !pBtCoexist->btLinkInfo.bHidOnly)
	{
		if(pBtCoexist->btLinkInfo.bA2dpOnly)	//A2DP
			{
			if(!bWifiBusy)
		     		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
			else //busy
			{
				if  (pCoexSta->nScanAPNum >= BT_8192E_1ANT_WIFI_NOISY_THRESH)  //no force LPS, no PS-TDMA, use pure TDMA
				{
 					halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
				}
				else
				{
					halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_LPS_ON, 0x50, 0x4);
				}
			}
		}
		else if ((pCoexSta->bPanExist == FALSE) && (pCoexSta->bA2dpExist == FALSE) && (pCoexSta->bHidExist == FALSE))
 			halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		else
			halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_LPS_ON, 0x50, 0x4);
	}
	else
		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);

	// tdma and coex table
	if(!bWifiBusy)
	{
		if(BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus)
		{
			halbtc8192e1ant_ActionWifiConnectedBtAclBusy(pBtCoexist,
				BT_8192E_1ANT_WIFI_STATUS_CONNECTED_IDLE);
		}
		else if( (BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
			(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
		{
			halbtc8192e1ant_ActionBtScoHidOnlyBusy(pBtCoexist,
				BT_8192E_1ANT_WIFI_STATUS_CONNECTED_IDLE);
		}
		else
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);

			if ( (pCoexSta->highPriorityTx) + (pCoexSta->highPriorityRx) <= 60 )
				halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
			else
				halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 7);
		}
	}
	else
	{
		if(BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus)
		{
			halbtc8192e1ant_ActionWifiConnectedBtAclBusy(pBtCoexist,
				BT_8192E_1ANT_WIFI_STATUS_CONNECTED_BUSY);
		}
		else if( (BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
			(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
		{
			halbtc8192e1ant_ActionBtScoHidOnlyBusy(pBtCoexist,
				BT_8192E_1ANT_WIFI_STATUS_CONNECTED_BUSY);
		}
		else
		{
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 8);

			if ( (pCoexSta->highPriorityTx) + (pCoexSta->highPriorityRx) <= 60 )
				halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
			else
				halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 7);
		}
	}
}

VOID
halbtc8192e1ant_RunSwCoexistMechanism(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	u1Byte				algorithm=0;

	algorithm = halbtc8192e1ant_ActionAlgorithm(pBtCoexist);
	pCoexDm->curAlgorithm = algorithm;

	if(halbtc8192e1ant_IsCommonAction(pBtCoexist))
	{

	}
	else
	{
		switch(pCoexDm->curAlgorithm)
		{
			case BT_8192E_1ANT_COEX_ALGO_SCO:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = SCO.\n"));
				//halbtc8192e1ant_ActionSco(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_HID:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = HID.\n"));
				//halbtc8192e1ant_ActionHid(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_A2DP:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = A2DP.\n"));
				//halbtc8192e1ant_ActionA2dp(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_A2DP_PANHS:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = A2DP+PAN(HS).\n"));
				//halbtc8192e1ant_ActionA2dpPanHs(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_PANEDR:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = PAN(EDR).\n"));
				//halbtc8192e1ant_ActionPanEdr(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_PANHS:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = HS mode.\n"));
				//halbtc8192e1ant_ActionPanHs(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_PANEDR_A2DP:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = PAN+A2DP.\n"));
				//halbtc8192e1ant_ActionPanEdrA2dp(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_PANEDR_HID:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = PAN(EDR)+HID.\n"));
				//halbtc8192e1ant_ActionPanEdrHid(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_HID_A2DP_PANEDR:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = HID+A2DP+PAN.\n"));
				//halbtc8192e1ant_ActionHidA2dpPanEdr(pBtCoexist);
				break;
			case BT_8192E_1ANT_COEX_ALGO_HID_A2DP:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = HID+A2DP.\n"));
				//halbtc8192e1ant_ActionHidA2dp(pBtCoexist);
				break;
			default:
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Action algorithm = coexist All Off!!\n"));
				//halbtc8192e1ant_CoexAllOff(pBtCoexist);
				break;
		}
		pCoexDm->preAlgorithm = pCoexDm->curAlgorithm;
	}
}

VOID
halbtc8192e1ant_RunCoexistMechanism(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BT_LINK_INFO pBtLinkInfo=&pBtCoexist->btLinkInfo;
	BOOLEAN	bWifiConnected=FALSE, bBtHsOn=FALSE;
	BOOLEAN	bIncreaseScanDevNum=FALSE;
	BOOLEAN	bBtCtrlAggBufSize=FALSE;
	BOOLEAN	bMiracastPlusBt=FALSE;
	u1Byte	aggBufSize=5;
	u1Byte	wifiRssiState=BTC_RSSI_STATE_HIGH;
	u4Byte	wifiLinkStatus=0;
	u4Byte	numOfWifiLink=0;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RunCoexistMechanism()===>\n"));

	if(pBtCoexist->bManualControl)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RunCoexistMechanism(), return for Manual CTRL <===\n"));
		return;
	}

	if(pBtCoexist->bStopCoexDm)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RunCoexistMechanism(), return for Stop Coex DM <===\n"));
		return;
	}

	if(pCoexSta->bUnderIps)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], wifi is under IPS !!!\n"));
		return;
	}

	if( (BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus) ||
		(BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
		(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
	{
		bIncreaseScanDevNum = TRUE;
	}

	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_INC_SCAN_DEV_NUM, &bIncreaseScanDevNum);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_LINK_STATUS, &wifiLinkStatus);
	numOfWifiLink = wifiLinkStatus>>16;

	if((numOfWifiLink>=2) || (wifiLinkStatus&WIFI_P2P_GO_CONNECTED))
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("############# [BTCoex],  Multi-Port numOfWifiLink = %d, wifiLinkStatus = 0x%x\n", numOfWifiLink,wifiLinkStatus) );

		if(pBtLinkInfo->bBtLinkExist)
		{
			halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 1, 1, 0, 1);
			bMiracastPlusBt = TRUE;
		}
		else
		{
			halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 0, 0, 0, 0);
			bMiracastPlusBt = FALSE;
		}
		pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_MIRACAST_PLUS_BT, &bMiracastPlusBt);
		halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, bBtCtrlAggBufSize, aggBufSize);

		if ( (pBtLinkInfo->bA2dpExist) && (pCoexSta->bC2hBtInquiryPage) )
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("############# [BTCoex],  BT Is Inquirying \n") );
			halbtc8192e1ant_ActionBtInquiry(pBtCoexist);
		}
		else
			halbtc8192e1ant_ActionWifiMultiPort(pBtCoexist);

		return;
	}
	else
	{
		bMiracastPlusBt = FALSE;
		pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_MIRACAST_PLUS_BT, &bMiracastPlusBt);
	}

	if ( (pBtLinkInfo->bBtLinkExist) && (bWifiConnected) )
	{
		halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 1, 1, 0, 1);

		if(pBtLinkInfo->bScoExist)
			halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, TRUE, 0x5);
		else
			halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, TRUE, 0x8);

		halbtc8192e1ant_SwMechanism(pBtCoexist, TRUE);
		halbtc8192e1ant_RunSwCoexistMechanism(pBtCoexist);  //just print debug message
	}
	else
	{
		halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 0, 0, 0, 0);

		halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, FALSE, 0x5);

		halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);
		halbtc8192e1ant_RunSwCoexistMechanism(pBtCoexist); ////just print debug message
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);
	if(pCoexSta->bC2hBtInquiryPage)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("############# [BTCoex],  BT Is Inquirying \n") );
		halbtc8192e1ant_ActionBtInquiry(pBtCoexist);
		return;
	}
	else if(bBtHsOn)
	{
		halbtc8192e1ant_ActionHs(pBtCoexist);
		return;
	}


	if(!bWifiConnected)
	{
		BOOLEAN	bScan=FALSE, bLink=FALSE, bRoam=FALSE;

		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], wifi is non connected-idle !!!\n"));

		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_SCAN, &bScan);
		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_LINK, &bLink);
		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_ROAM, &bRoam);

		if(bScan || bLink || bRoam)
		{
			 if (bScan)
		   		halbtc8192e1ant_ActionWifiNotConnectedScan(pBtCoexist);
			 else
		    		halbtc8192e1ant_ActionWifiNotConnectedAssoAuth(pBtCoexist);
		}
		else
			halbtc8192e1ant_ActionWifiNotConnected(pBtCoexist);
	}
	else	// wifi LPS/Busy
	{
		halbtc8192e1ant_ActionWifiConnected(pBtCoexist);
	}
}

VOID
halbtc8192e1ant_InitCoexDm(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	// force to reset coex mechanism

	// sw all off
	halbtc8192e1ant_SwMechanism(pBtCoexist, FALSE);

	//halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, FALSE, 8);
	halbtc8192e1ant_CoexTableWithType(pBtCoexist, FORCE_EXEC, 0);

	pCoexSta->popEventCnt = 0;
}

VOID
halbtc8192e1ant_InitHwConfig(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bWifiOnly
	)
{
	u4Byte	u4Tmp=0;
	u2Byte	u2Tmp=0;
	u1Byte	u1Tmp=0;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], 1Ant Init HW Config!!\n"));

	// antenna sw ctrl to bt
	halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, TRUE, FALSE);

	halbtc8192e1ant_CoexTableWithType(pBtCoexist, FORCE_EXEC, 0);

	// antenna switch control parameter
	pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x858, 0x55555555);

	// coex parameters
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x778, 0x1);
	// 0x790[5:0]=0x5
	u1Tmp = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x790);
	u1Tmp &= 0xc0;
	u1Tmp |= 0x5;
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x790, u1Tmp);

	// enable counter statistics
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x76e, 0x4);

	// enable PTA
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x40, 0x20);
	// enable mailbox interface
	u2Tmp = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0x40);
	u2Tmp |= BIT9;
	pBtCoexist->fBtcWrite2Byte(pBtCoexist, 0x40, u2Tmp);

	// enable PTA I2C mailbox
	u1Tmp = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x101);
	u1Tmp |= BIT4;
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x101, u1Tmp);

	// enable bt clock when wifi is disabled.
	u1Tmp = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x93);
	u1Tmp |= BIT0;
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x93, u1Tmp);
	// enable bt clock when suspend.
	u1Tmp = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x7);
	u1Tmp |= BIT0;
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x7, u1Tmp);
}


/*
VOID
halbtc8192e1ant_WifiOffHwCfg(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	// set wlan_act to low
	//pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x76e, 0x4);
}
*/

//============================================================
// work around function start with wa_halbtc8192e1ant_
//============================================================
//============================================================
// extern function start with EXhalbtc8192e1ant_
//============================================================
VOID
EXhalbtc8192e1ant_PowerOnSetting(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
#if 0
	PBTC_BOARD_INFO 	pBoardInfo=&pBtCoexist->boardInfo;
	u1Byte u1Tmp=0x0;
	u2Byte u2Tmp=0x0;

	pBtCoexist->bStopCoexDm = TRUE;

	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x67, 0x20);

	// enable BB, REG_SYS_FUNC_EN such that we can write 0x948 correctly.
	u2Tmp = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0x2);
	pBtCoexist->fBtcWrite2Byte(pBtCoexist, 0x2, u2Tmp|BIT0|BIT1);

	// set GRAN_BT = 1
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x765, 0x18);
	// set WLAN_ACT = 0
	pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x76e, 0x4);

	//
	// S0 or S1 setting and Local register setting(By the setting fw can get ant number, S0/S1, ... info)
	// Local setting bit define
	//	BIT0: "0" for no antenna inverse; "1" for antenna inverse
	//	BIT1: "0" for internal switch; "1" for external switch
	//	BIT2: "0" for one antenna; "1" for two antenna
	// NOTE: here default all internal switch and 1-antenna ==> BIT1=0 and BIT2=0
	if(pBtCoexist->chipInterface == BTC_INTF_USB)
	{
		// fixed at S0 for USB interface
		pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x948, 0x0);

	 	u1Tmp |= 0x1;	// antenna inverse
		pBtCoexist->fBtcWriteLocalReg1Byte(pBtCoexist, 0xfe08, u1Tmp);

		pBoardInfo->btdmAntPos = BTC_ANTENNA_AT_AUX_PORT;
	}
	else
	{
		// for PCIE and SDIO interface, we check efuse 0xc3[6]
		if(pBoardInfo->singleAntPath == 0)
		{
			// set to S1
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x948, 0x280);
			pBoardInfo->btdmAntPos = BTC_ANTENNA_AT_MAIN_PORT;
		}
		else if(pBoardInfo->singleAntPath == 1)
		{
			// set to S0
			pBtCoexist->fBtcWrite4Byte(pBtCoexist, 0x948, 0x0);
			u1Tmp |= 0x1;	// antenna inverse
			pBoardInfo->btdmAntPos = BTC_ANTENNA_AT_AUX_PORT;
		}

		if(pBtCoexist->chipInterface == BTC_INTF_PCI)
		{
			pBtCoexist->fBtcWriteLocalReg1Byte(pBtCoexist, 0x384, u1Tmp);
		}
		else if(pBtCoexist->chipInterface == BTC_INTF_SDIO)
		{
			pBtCoexist->fBtcWriteLocalReg1Byte(pBtCoexist, 0x60, u1Tmp);
		}
	}
#endif
}

VOID
EXhalbtc8192e1ant_PreLoadFirmware(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
}

VOID
EXhalbtc8192e1ant_InitHwConfig(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	BOOLEAN				bWifiOnly
	)
{
	halbtc8192e1ant_InitHwConfig(pBtCoexist, bWifiOnly);
	pBtCoexist->bStopCoexDm = FALSE;
}

VOID
EXhalbtc8192e1ant_InitCoexDm(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Coex Mechanism Init!!\n"));

	pBtCoexist->bStopCoexDm = FALSE;

	halbtc8192e1ant_InitCoexDm(pBtCoexist);

	halbtc8192e1ant_QueryBtInfo(pBtCoexist);
}

VOID
EXhalbtc8192e1ant_DisplayCoexInfo(
	IN	PBTC_COEXIST		pBtCoexist
	)
{
	PBTC_BOARD_INFO		pBoardInfo=&pBtCoexist->boardInfo;
	PBTC_STACK_INFO		pStackInfo=&pBtCoexist->stackInfo;
	PBTC_BT_LINK_INFO	pBtLinkInfo=&pBtCoexist->btLinkInfo;
	pu1Byte				cliBuf=pBtCoexist->cliBuf;
	u1Byte				u1Tmp[4], i, btInfoExt, psTdmaCase=0;
	u4Byte				u4Tmp[4];
	u4Byte				fwVer=0, btPatchVer=0;

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n ============[BT Coexist info]============");
	CL_PRINTF(cliBuf);

	if(pBtCoexist->bManualControl)
	{
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n ============[Under Manual Control]============");
		CL_PRINTF(cliBuf);
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n ==========================================");
		CL_PRINTF(cliBuf);
	}
	if(pBtCoexist->bStopCoexDm)
	{
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n ============[Coex is STOPPED]============");
		CL_PRINTF(cliBuf);
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n ==========================================");
		CL_PRINTF(cliBuf);
	}

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d/ %d ", "Ant PG number/ Ant mechanism:", \
		pBoardInfo->pgAntNum, pBoardInfo->btdmAntNum);
	CL_PRINTF(cliBuf);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %s / %d", "BT stack/ hci ext ver", \
		((pStackInfo->bProfileNotified)? "Yes":"No"), pStackInfo->hciVersion);
	CL_PRINTF(cliBuf);

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_BT_PATCH_VER, &btPatchVer);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_FW_VER, &fwVer);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d_%d/ 0x%x/ 0x%x(%d)", "CoexVer/ FwVer/ PatchVer", \
		GLCoexVerDate8192e1Ant, GLCoexVer8192e1Ant, fwVer, btPatchVer, btPatchVer);
	CL_PRINTF(cliBuf);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %02x %02x %02x ", "Wifi channel informed to BT", \
		pCoexDm->wifiChnlInfo[0], pCoexDm->wifiChnlInfo[1],
		pCoexDm->wifiChnlInfo[2]);
	CL_PRINTF(cliBuf);

	// wifi status
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s", "============[Wifi Status]============");
	CL_PRINTF(cliBuf);
	pBtCoexist->fBtcDispDbgMsg(pBtCoexist, BTC_DBG_DISP_WIFI_STATUS);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s", "============[BT Status]============");
	CL_PRINTF(cliBuf);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = [%s/ %d/ %d] ", "BT [status/ rssi/ retryCnt]", \
		((pBtCoexist->btInfo.bBtDisabled)? ("disabled"):	((pCoexSta->bC2hBtInquiryPage)?("inquiry/page scan"):((BT_8192E_1ANT_BT_STATUS_NON_CONNECTED_IDLE == pCoexDm->btStatus)? "non-connected idle":
		(  (BT_8192E_1ANT_BT_STATUS_CONNECTED_IDLE == pCoexDm->btStatus)? "connected-idle":"busy")))),
		pCoexSta->btRssi, pCoexSta->btRetryCnt);
	CL_PRINTF(cliBuf);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d / %d / %d / %d", "SCO/HID/PAN/A2DP", \
		pBtLinkInfo->bScoExist, pBtLinkInfo->bHidExist, pBtLinkInfo->bPanExist, pBtLinkInfo->bA2dpExist);
	CL_PRINTF(cliBuf);
	pBtCoexist->fBtcDispDbgMsg(pBtCoexist, BTC_DBG_DISP_BT_LINK_INFO);

	btInfoExt = pCoexSta->btInfoExt;
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %s", "BT Info A2DP rate", \
		(btInfoExt&BIT0)? "Basic rate":"EDR rate");
	CL_PRINTF(cliBuf);

	for(i=0; i<BT_INFO_SRC_8192E_1ANT_MAX; i++)
	{
		if(pCoexSta->btInfoC2hCnt[i])
		{
			CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %02x %02x %02x %02x %02x %02x %02x(%d)", GLBtInfoSrc8192e1Ant[i], \
				pCoexSta->btInfoC2h[i][0], pCoexSta->btInfoC2h[i][1],
				pCoexSta->btInfoC2h[i][2], pCoexSta->btInfoC2h[i][3],
				pCoexSta->btInfoC2h[i][4], pCoexSta->btInfoC2h[i][5],
				pCoexSta->btInfoC2h[i][6], pCoexSta->btInfoC2hCnt[i]);
			CL_PRINTF(cliBuf);
		}
	}

	if(!pBtCoexist->bManualControl)
	{
		// Sw mechanism
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s", "============[Sw mechanism]============");
		CL_PRINTF(cliBuf);

		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %s/ %s/ %d ", "DelBA/ BtCtrlAgg/ AggSize", \
			(pBtCoexist->btInfo.bRejectAggPkt? "Yes":"No"), (pBtCoexist->btInfo.bBtCtrlAggBufSize? "Yes":"No"),
				pBtCoexist->btInfo.aggBufSize);
		CL_PRINTF(cliBuf);
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x ", "Rate Mask", \
				pBtCoexist->btInfo.raMask);
		CL_PRINTF(cliBuf);

		// Fw mechanism
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s", "============[Fw mechanism]============");
		CL_PRINTF(cliBuf);

		psTdmaCase = pCoexDm->curPsTdma;
		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %02x %02x %02x %02x %02x case-%d (auto:%d)", "PS TDMA", \
			pCoexDm->psTdmaPara[0], pCoexDm->psTdmaPara[1],
			pCoexDm->psTdmaPara[2], pCoexDm->psTdmaPara[3],
			pCoexDm->psTdmaPara[4], psTdmaCase, pCoexDm->bAutoTdmaAdjust);
		CL_PRINTF(cliBuf);

		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x ", "Latest error condition(should be 0)", \
			pCoexDm->errorCondition);
		CL_PRINTF(cliBuf);

		CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d ", "IgnWlanAct", \
			pCoexDm->bCurIgnoreWlanAct);
		CL_PRINTF(cliBuf);
	}

	// Hw setting
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s", "============[Hw setting]============");
	CL_PRINTF(cliBuf);

	u4Tmp[0] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0xc04);
	u4Tmp[1] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0xd04);
	u4Tmp[2] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x90c);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x/ 0x%x/ 0x%x", "0xc04/ 0xd04/ 0x90c", \
		u4Tmp[0], u4Tmp[1], u4Tmp[2]);
	CL_PRINTF(cliBuf);

	u1Tmp[0] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x778);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x", "0x778", \
		u1Tmp[0]);
	CL_PRINTF(cliBuf);

	u1Tmp[0] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x92c);
	u4Tmp[0] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x930);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x/ 0x%x", "0x92c/ 0x930", \
		(u1Tmp[0]), u4Tmp[0]);
	CL_PRINTF(cliBuf);

	u1Tmp[0] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x40);
	u1Tmp[1] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x4f);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x/ 0x%x", "0x40/ 0x4f", \
		u1Tmp[0], u1Tmp[1]);
	CL_PRINTF(cliBuf);

	u4Tmp[0] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x550);
	u1Tmp[0] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x522);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x/ 0x%x", "0x550(bcn ctrl)/0x522", \
		u4Tmp[0], u1Tmp[0]);
	CL_PRINTF(cliBuf);

	u4Tmp[0] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0xc50);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x", "0xc50(dig)", \
		u4Tmp[0]);
	CL_PRINTF(cliBuf);

	u4Tmp[0] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x6c0);
	u4Tmp[1] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x6c4);
	u4Tmp[2] = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x6c8);
	u1Tmp[0] = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x6cc);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = 0x%x/ 0x%x/ 0x%x/ 0x%x", "0x6c0/0x6c4/0x6c8/0x6cc(coexTable)", \
		u4Tmp[0], u4Tmp[1], u4Tmp[2], u1Tmp[0]);
	CL_PRINTF(cliBuf);

	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d/ %d", "0x770(hp rx[31:16]/tx[15:0])", \
		pCoexSta->highPriorityRx, pCoexSta->highPriorityTx);
	CL_PRINTF(cliBuf);
	CL_SPRINTF(cliBuf, BT_TMP_BUF_SIZE, "\r\n %-35s = %d/ %d", "0x774(lp rx[31:16]/tx[15:0])", \
		pCoexSta->lowPriorityRx, pCoexSta->lowPriorityTx);
	CL_PRINTF(cliBuf);
#if(BT_AUTO_REPORT_ONLY_8192E_1ANT == 1)
	halbtc8192e1ant_MonitorBtCtr(pBtCoexist);
#endif

	pBtCoexist->fBtcDispDbgMsg(pBtCoexist, BTC_DBG_DISP_COEX_STATISTICS);
}

VOID
EXhalbtc8192e1ant_IpsNotify(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			type
	)
{
	u4Byte	u4Tmp=0;

	if(pBtCoexist->bManualControl ||	pBtCoexist->bStopCoexDm)
		return;

	if(BTC_IPS_ENTER == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], IPS ENTER notify\n"));
		pCoexSta->bUnderIps = TRUE;

		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 0);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 0);
		halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, FALSE, TRUE);
	}
	else if(BTC_IPS_LEAVE == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], IPS LEAVE notify\n"));
		pCoexSta->bUnderIps = FALSE;

		halbtc8192e1ant_InitHwConfig(pBtCoexist, FALSE);
		halbtc8192e1ant_InitCoexDm(pBtCoexist);
		halbtc8192e1ant_QueryBtInfo(pBtCoexist);
	}
}

VOID
EXhalbtc8192e1ant_LpsNotify(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			type
	)
{
	if(pBtCoexist->bManualControl || pBtCoexist->bStopCoexDm)
		return;

	if(BTC_LPS_ENABLE == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], LPS ENABLE notify\n"));
		pCoexSta->bUnderLps = TRUE;
	}
	else if(BTC_LPS_DISABLE == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], LPS DISABLE notify\n"));
		pCoexSta->bUnderLps = FALSE;
	}
}

VOID
EXhalbtc8192e1ant_ScanNotify(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			type
	)
{
	BOOLEAN bWifiConnected=FALSE, bBtHsOn=FALSE;
	u4Byte	wifiLinkStatus=0;
	u4Byte	numOfWifiLink=0;
	BOOLEAN	bBtCtrlAggBufSize=FALSE;
	u1Byte	aggBufSize=5;

	u1Byte u1Tmpa, u1Tmpb;
	u4Byte u4Tmp;

	if(pBtCoexist->bManualControl ||
		pBtCoexist->bStopCoexDm )
		return;

	if(BTC_SCAN_START == type)
	{
		pCoexSta->bWiFiIsHighPriTask = TRUE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], SCAN START notify\n"));

		halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, FALSE, 8);  //Force antenna setup for no scan result issue
		u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x948);
		u1Tmpa = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x765);
		u1Tmpb = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x67);


		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], 0x948=0x%x, 0x765=0x%x, 0x67=0x%x\n",
			u4Tmp,  u1Tmpa, u1Tmpb));
	}
	else
	{
		pCoexSta->bWiFiIsHighPriTask = FALSE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], SCAN FINISH notify\n"));

		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U1_AP_NUM, &pCoexSta->nScanAPNum);
	}

	if(pBtCoexist->btInfo.bBtDisabled)
		return;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);

	halbtc8192e1ant_QueryBtInfo(pBtCoexist);

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_LINK_STATUS, &wifiLinkStatus);
	numOfWifiLink = wifiLinkStatus>>16;
	if(numOfWifiLink >= 2)
	{
		halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 0, 0, 0, 0);
		halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, bBtCtrlAggBufSize, aggBufSize);
		halbtc8192e1ant_ActionWifiMultiPort(pBtCoexist);
		return;
	}

	if(pCoexSta->bC2hBtInquiryPage)
	{
		halbtc8192e1ant_ActionBtInquiry(pBtCoexist);
		return;
	}
	else if(bBtHsOn)
	{
		halbtc8192e1ant_ActionHs(pBtCoexist);
		return;
	}

	if(BTC_SCAN_START == type)
	{
		//RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], SCAN START notify\n"));
		if(!bWifiConnected)	// non-connected scan
		{
			halbtc8192e1ant_ActionWifiNotConnectedScan(pBtCoexist);
		}
		else	// wifi is connected
		{
			halbtc8192e1ant_ActionWifiConnectedScan(pBtCoexist);
		}
	}
	else if(BTC_SCAN_FINISH == type)
	{
		//RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], SCAN FINISH notify\n"));
		if(!bWifiConnected)	// non-connected scan
		{
			halbtc8192e1ant_ActionWifiNotConnected(pBtCoexist);
		}
		else
		{
			halbtc8192e1ant_ActionWifiConnected(pBtCoexist);
		}
	}
}

VOID
EXhalbtc8192e1ant_ConnectNotify(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	u1Byte			type
	)
{
	BOOLEAN	bWifiConnected=FALSE, bBtHsOn=FALSE;
	u4Byte	wifiLinkStatus=0;
	u4Byte	numOfWifiLink=0;
	BOOLEAN	bBtCtrlAggBufSize=FALSE;
	u1Byte	aggBufSize=5;

	if(pBtCoexist->bManualControl ||
		pBtCoexist->bStopCoexDm ||
		pBtCoexist->btInfo.bBtDisabled )
		return;

	if(BTC_ASSOCIATE_START == type)
	{
		 pCoexSta->bWiFiIsHighPriTask = TRUE;
		 RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CONNECT START notify\n"));
		 pCoexDm->nArpCnt = 0;
	}
	else
	{
		pCoexSta->bWiFiIsHighPriTask = FALSE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CONNECT FINISH notify\n"));
		//pCoexDm->nArpCnt = 0;
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_LINK_STATUS, &wifiLinkStatus);
	numOfWifiLink = wifiLinkStatus>>16;
	if(numOfWifiLink >= 2)
	{
		halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 0, 0, 0, 0);
		halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, bBtCtrlAggBufSize, aggBufSize);
		halbtc8192e1ant_ActionWifiMultiPort(pBtCoexist);
		return;
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);
	if(pCoexSta->bC2hBtInquiryPage)
	{
		halbtc8192e1ant_ActionBtInquiry(pBtCoexist);
		return;
	}
	else if(bBtHsOn)
	{
		halbtc8192e1ant_ActionHs(pBtCoexist);
		return;
	}

	if(BTC_ASSOCIATE_START == type)
	{
		//RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CONNECT START notify\n"));
		halbtc8192e1ant_ActionWifiNotConnectedAssoAuth(pBtCoexist);
	}
	else if(BTC_ASSOCIATE_FINISH == type)
	{
		//RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CONNECT FINISH notify\n"));

		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);
		if(!bWifiConnected) // non-connected scan
		{
			halbtc8192e1ant_ActionWifiNotConnected(pBtCoexist);
		}
		else
		{
			halbtc8192e1ant_ActionWifiConnected(pBtCoexist);
		}
	}
}

VOID
EXhalbtc8192e1ant_MediaStatusNotify(
	IN	PBTC_COEXIST			pBtCoexist,
	IN	u1Byte				type
	)
{
	u1Byte			H2C_Parameter[3] ={0};
	u4Byte			wifiBw;
	u1Byte			wifiCentralChnl;
	BOOLEAN			bWifiUnderBMode = FALSE;

	if(pBtCoexist->bManualControl ||
		pBtCoexist->bStopCoexDm ||
		pBtCoexist->btInfo.bBtDisabled )
		return;

	if(BTC_MEDIA_CONNECT == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], MEDIA connect notify\n"));

		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_UNDER_B_MODE, &bWifiUnderBMode);

		//Set CCK Tx/Rx high Pri except 11b mode
		if (bWifiUnderBMode)
		{
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cd, 0x00); //CCK Tx
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cf, 0x00); //CCK Rx
		}
		else
		{
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cd, 0x10); //CCK Tx
			pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cf, 0x10); //CCK Rx
		}

		pCoexDm->backupArfrCnt1 = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x430);
		pCoexDm->backupArfrCnt2 = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x434);
		pCoexDm->backupRetryLimit = pBtCoexist->fBtcRead2Byte(pBtCoexist, 0x42a);
		pCoexDm->backupAmpduMaxTime = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x456);
	}
	else
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], MEDIA disconnect notify\n"));
		pCoexDm->nArpCnt = 0;

		pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cd, 0x0); //CCK Tx
		pBtCoexist->fBtcWrite1Byte(pBtCoexist, 0x6cf, 0x0); //CCK Rx
	}

	// only 2.4G we need to inform bt the chnl mask
	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U1_WIFI_CENTRAL_CHNL, &wifiCentralChnl);
	if( (BTC_MEDIA_CONNECT == type) &&
		(wifiCentralChnl <= 14) )
	{
		//H2C_Parameter[0] = 0x1;
		H2C_Parameter[0] = 0x0;
		H2C_Parameter[1] = wifiCentralChnl;
		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_BW, &wifiBw);
		if(BTC_WIFI_BW_HT40 == wifiBw)
			H2C_Parameter[2] = 0x30;
		else
			H2C_Parameter[2] = 0x20;
	}

	pCoexDm->wifiChnlInfo[0] = H2C_Parameter[0];
	pCoexDm->wifiChnlInfo[1] = H2C_Parameter[1];
	pCoexDm->wifiChnlInfo[2] = H2C_Parameter[2];

	RT_TRACE(COMP_COEX, DBG_TRACE, ("[BTCoex], FW write 0x66=0x%x\n",
		H2C_Parameter[0]<<16|H2C_Parameter[1]<<8|H2C_Parameter[2]));

	pBtCoexist->fBtcFillH2c(pBtCoexist, 0x66, 3, H2C_Parameter);
}

VOID
EXhalbtc8192e1ant_SpecialPacketNotify(
	IN	PBTC_COEXIST			pBtCoexist,
	IN	u1Byte				type
	)
{
	BOOLEAN	bBtHsOn=FALSE;
	u4Byte	wifiLinkStatus=0;
	u4Byte	numOfWifiLink=0;
	BOOLEAN	bBtCtrlAggBufSize=FALSE;
	u1Byte	aggBufSize=5;

	if(pBtCoexist->bManualControl ||
		pBtCoexist->bStopCoexDm ||
		pBtCoexist->btInfo.bBtDisabled )
		return;

	if( BTC_PACKET_DHCP == type ||
		BTC_PACKET_EAPOL == type ||
		BTC_PACKET_ARP == type )
	{
		if(BTC_PACKET_ARP == type)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], special Packet ARP notify\n"));

			pCoexDm->nArpCnt++;
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ARP Packet Count = %d\n", pCoexDm->nArpCnt));

			if(pCoexDm->nArpCnt >= 10) // if APR PKT > 10 after connect, do not go to ActionWifiConnectedSpecialPacket(pBtCoexist)
			{
				pCoexSta->bWiFiIsHighPriTask = FALSE;
			}
			else
			{
				pCoexSta->bWiFiIsHighPriTask = TRUE;
			}
		}
		else
		{
			pCoexSta->bWiFiIsHighPriTask = TRUE;
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], special Packet DHCP or EAPOL notify\n"));
		}
	}
	else
	{
		pCoexSta->bWiFiIsHighPriTask = FALSE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], special Packet [Type = %d] notify\n", type));
	}

	pCoexSta->specialPktPeriodCnt = 0;

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_LINK_STATUS, &wifiLinkStatus);
	numOfWifiLink = wifiLinkStatus>>16;
	if(numOfWifiLink >= 2)
	{
		halbtc8192e1ant_LimitedTx(pBtCoexist, NORMAL_EXEC, 0, 0, 0, 0);
		halbtc8192e1ant_LimitedRx(pBtCoexist, NORMAL_EXEC, FALSE, bBtCtrlAggBufSize, aggBufSize);
		halbtc8192e1ant_ActionWifiMultiPort(pBtCoexist);
		return;
	}

	pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_HS_OPERATION, &bBtHsOn);
	if(pCoexSta->bC2hBtInquiryPage)
	{
		halbtc8192e1ant_ActionBtInquiry(pBtCoexist);
		return;
	}
	else if(bBtHsOn)
	{
		halbtc8192e1ant_ActionHs(pBtCoexist);
		return;
	}

	if( BTC_PACKET_DHCP == type ||
		BTC_PACKET_EAPOL == type ||
		( (BTC_PACKET_ARP == type ) && (pCoexSta->bWiFiIsHighPriTask) ) )
	{
		halbtc8192e1ant_ActionWifiConnectedSpecialPacket(pBtCoexist);
	}
}

VOID
EXhalbtc8192e1ant_BtInfoNotify(
	IN	PBTC_COEXIST		pBtCoexist,
	IN	pu1Byte			tmpBuf,
	IN	u1Byte			length
	)
{
	PBTC_BT_LINK_INFO	pBtLinkInfo=&pBtCoexist->btLinkInfo;
	u1Byte				btInfo=0;
	u1Byte				i, rspSource=0;
	BOOLEAN				bWifiConnected=FALSE;
	BOOLEAN				bBtBusy=FALSE;

	pCoexSta->bC2hBtInfoReqSent = FALSE;

	rspSource = tmpBuf[0]&0xf;
	if(rspSource >= BT_INFO_SRC_8192E_1ANT_MAX)
		rspSource = BT_INFO_SRC_8192E_1ANT_WIFI_FW;
	pCoexSta->btInfoC2hCnt[rspSource]++;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Bt info[%d], length=%d, hex data=[", rspSource, length));
	for(i=0; i<length; i++)
	{
		pCoexSta->btInfoC2h[rspSource][i] = tmpBuf[i];
		if(i == 1)
			btInfo = tmpBuf[i];
		if(i == length-1)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("0x%02x]\n", tmpBuf[i]));
		}
		else
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("0x%02x, ", tmpBuf[i]));
		}
	}

	if(BT_INFO_SRC_8192E_1ANT_WIFI_FW != rspSource)
	{
		pCoexSta->btRetryCnt =	// [3:0]
			pCoexSta->btInfoC2h[rspSource][2]&0xf;

		if (pCoexSta->btRetryCnt >= 1)
			pCoexSta->popEventCnt++;

		if (pCoexSta->btInfoC2h[rspSource][2]&0x20)
			pCoexSta->bC2hBtPage = TRUE;
		else
			pCoexSta->bC2hBtPage = FALSE;

		pCoexSta->btRssi =
			pCoexSta->btInfoC2h[rspSource][3]*2-90;
			//pCoexSta->btInfoC2h[rspSource][3]*2+10;

		pCoexSta->btInfoExt =
			pCoexSta->btInfoC2h[rspSource][4];

		pCoexSta->bBtTxRxMask = (pCoexSta->btInfoC2h[rspSource][2]&0x40);
		pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_TX_RX_MASK, &pCoexSta->bBtTxRxMask);
		if(!pCoexSta->bBtTxRxMask)
		{
			/* BT into is responded by BT FW and BT RF REG 0x3C != 0x15 => Need to switch BT TRx Mask */
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Switch BT TRx Mask since BT RF REG 0x3C != 0x15\n"));
			pBtCoexist->fBtcSetBtReg(pBtCoexist, BTC_BT_REG_RF, 0x3c, 0x15);
		}

		// Here we need to resend some wifi info to BT
		// because bt is reset and loss of the info.
		if(pCoexSta->btInfoExt & BIT1)
		{
			RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT ext info bit1 check, send wifi BW&Chnl to BT!!\n"));
			pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_BL_WIFI_CONNECTED, &bWifiConnected);
			if(bWifiConnected)
			{
				EXhalbtc8192e1ant_MediaStatusNotify(pBtCoexist, BTC_MEDIA_CONNECT);
			}
			else
			{
				EXhalbtc8192e1ant_MediaStatusNotify(pBtCoexist, BTC_MEDIA_DISCONNECT);
			}
		}

		if(pCoexSta->btInfoExt & BIT3)
		{
			if(!pBtCoexist->bManualControl && !pBtCoexist->bStopCoexDm)
			{
				RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT ext info bit3 check, set BT NOT to ignore Wlan active!!\n"));
				halbtc8192e1ant_IgnoreWlanAct(pBtCoexist, FORCE_EXEC, FALSE);
			}
		}
		else
		{
			// BT already NOT ignore Wlan active, do nothing here.
		}
#if(BT_AUTO_REPORT_ONLY_8192E_1ANT == 0)
		if( (pCoexSta->btInfoExt & BIT4) )
		{
			// BT auto report already enabled, do nothing
		}
		else
		{
			halbtc8192e1ant_BtAutoReport(pBtCoexist, FORCE_EXEC, TRUE);
		}
#endif
	}

	// check BIT2 first ==> check if bt is under inquiry or page scan
	if(btInfo & BT_INFO_8192E_1ANT_B_INQ_PAGE)
		pCoexSta->bC2hBtInquiryPage = TRUE;
	else
		pCoexSta->bC2hBtInquiryPage = FALSE;

	// set link exist status
	if(!(btInfo&BT_INFO_8192E_1ANT_B_CONNECTION))
	{
		pCoexSta->bBtLinkExist = FALSE;
		pCoexSta->bPanExist = FALSE;
		pCoexSta->bA2dpExist = FALSE;
		pCoexSta->bHidExist = FALSE;
		pCoexSta->bScoExist = FALSE;
	}
	else	// connection exists
	{
		pCoexSta->bBtLinkExist = TRUE;
		if(btInfo & BT_INFO_8192E_1ANT_B_FTP)
			pCoexSta->bPanExist = TRUE;
		else
			pCoexSta->bPanExist = FALSE;
		if(btInfo & BT_INFO_8192E_1ANT_B_A2DP)
			pCoexSta->bA2dpExist = TRUE;
		else
			pCoexSta->bA2dpExist = FALSE;
		if(btInfo & BT_INFO_8192E_1ANT_B_HID)
			pCoexSta->bHidExist = TRUE;
		else
			pCoexSta->bHidExist = FALSE;
		if(btInfo & BT_INFO_8192E_1ANT_B_SCO_ESCO)
			pCoexSta->bScoExist = TRUE;
		else
			pCoexSta->bScoExist = FALSE;
	}

	halbtc8192e1ant_UpdateBtLinkInfo(pBtCoexist);

	btInfo = btInfo & 0x1f;  //mask profile bit for connect-ilde identification ( for CSR case: A2DP idle --> 0x41)

	if(!(btInfo&BT_INFO_8192E_1ANT_B_CONNECTION))
	{
		pCoexDm->btStatus = BT_8192E_1ANT_BT_STATUS_NON_CONNECTED_IDLE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BtInfoNotify(), BT Non-Connected idle!!!\n"));
	}
	else if(btInfo == BT_INFO_8192E_1ANT_B_CONNECTION)	// connection exists but no busy
	{
		pCoexDm->btStatus = BT_8192E_1ANT_BT_STATUS_CONNECTED_IDLE;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BtInfoNotify(), BT Connected-idle!!!\n"));
	}
	else if((btInfo&BT_INFO_8192E_1ANT_B_SCO_ESCO) ||
		(btInfo&BT_INFO_8192E_1ANT_B_SCO_BUSY))
	{
		pCoexDm->btStatus = BT_8192E_1ANT_BT_STATUS_SCO_BUSY;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BtInfoNotify(), BT SCO busy!!!\n"));
	}
	else if(btInfo&BT_INFO_8192E_1ANT_B_ACL_BUSY)
	{
		if(BT_8192E_1ANT_BT_STATUS_ACL_BUSY != pCoexDm->btStatus)
			pCoexDm->bAutoTdmaAdjust = FALSE;
		pCoexDm->btStatus = BT_8192E_1ANT_BT_STATUS_ACL_BUSY;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BtInfoNotify(), BT ACL busy!!!\n"));
	}
	else
	{
		pCoexDm->btStatus = BT_8192E_1ANT_BT_STATUS_MAX;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BtInfoNotify(), BT Non-Defined state!!!\n"));
	}

	if( (BT_8192E_1ANT_BT_STATUS_ACL_BUSY == pCoexDm->btStatus) ||
		(BT_8192E_1ANT_BT_STATUS_SCO_BUSY == pCoexDm->btStatus) ||
		(BT_8192E_1ANT_BT_STATUS_ACL_SCO_BUSY == pCoexDm->btStatus) )
		bBtBusy = TRUE;
	else
		bBtBusy = FALSE;
	pBtCoexist->fBtcSet(pBtCoexist, BTC_SET_BL_BT_TRAFFIC_BUSY, &bBtBusy);

	halbtc8192e1ant_RunCoexistMechanism(pBtCoexist);
}

VOID
EXhalbtc8192e1ant_RfStatusNotify(
	IN	PBTC_COEXIST			pBtCoexist,
	IN	u1Byte					type
	)
{
	u4Byte	u4Tmp;
	u1Byte	u1Tmpa,u1Tmpb, u1Tmpc;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RF Status notify\n"));

	if(BTC_RF_ON == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RF is turned ON!!\n"));
		pBtCoexist->bStopCoexDm = FALSE;
	}
	else if(BTC_RF_OFF == type)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], RF is turned OFF!!\n"));

		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, FALSE, 0);
		halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, FALSE, TRUE);

		halbtc8192e1ant_IgnoreWlanAct(pBtCoexist, FORCE_EXEC, TRUE);
		pBtCoexist->bStopCoexDm = TRUE;

		u4Tmp = pBtCoexist->fBtcRead4Byte(pBtCoexist, 0x948);
		u1Tmpa = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x765);
 		u1Tmpb = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x67);
		u1Tmpc = pBtCoexist->fBtcRead1Byte(pBtCoexist, 0x76e);


		RT_TRACE(COMP_COEX, DBG_LOUD, ("############# [BTCoex], 0x948=0x%x, 0x765=0x%x, 0x67=0x%x, 0x76e=0x%x\n",
			u4Tmp,  u1Tmpa, u1Tmpb, u1Tmpc));

	}
}

VOID
EXhalbtc8192e1ant_HaltNotify(
	IN	PBTC_COEXIST			pBtCoexist
	)
{
	u4Byte	u4Tmp;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Halt notify\n"));

	halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
	halbtc8192e1ant_PsTdma(pBtCoexist, FORCE_EXEC, FALSE, 0);
	halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, FALSE, TRUE);

	halbtc8192e1ant_IgnoreWlanAct(pBtCoexist, FORCE_EXEC, TRUE);

	EXhalbtc8192e1ant_MediaStatusNotify(pBtCoexist, BTC_MEDIA_DISCONNECT);

	pBtCoexist->bStopCoexDm = TRUE;
}

VOID
EXhalbtc8192e1ant_PnpNotify(
	IN	PBTC_COEXIST			pBtCoexist,
	IN	u1Byte				pnpState
	)
{
	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Pnp notify\n"));

	if(BTC_WIFI_PNP_SLEEP == pnpState)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Pnp notify to SLEEP\n"));

		halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
		halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 0);
		halbtc8192e1ant_CoexTableWithType(pBtCoexist, NORMAL_EXEC, 2);
		halbtc8192e1ant_SetAntPath(pBtCoexist, BTC_ANT_PATH_BT, FALSE, TRUE);

		pBtCoexist->bStopCoexDm = TRUE;
	}
	else if(BTC_WIFI_PNP_WAKE_UP == pnpState)
	{
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Pnp notify to WAKE UP\n"));
		pBtCoexist->bStopCoexDm = FALSE;
		halbtc8192e1ant_InitHwConfig(pBtCoexist, FALSE);
		halbtc8192e1ant_InitCoexDm(pBtCoexist);
		halbtc8192e1ant_QueryBtInfo(pBtCoexist);
	}
}

VOID
EXhalbtc8192e1ant_CoexDmReset(
	IN	PBTC_COEXIST			pBtCoexist
	)
{
	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], *****************Coex DM Reset*****************\n"));

	halbtc8192e1ant_InitHwConfig(pBtCoexist, FALSE);
	//pBtCoexist->fBtcSetRfReg(pBtCoexist, BTC_RF_A, 0x1, 0xfffff, 0x0);
	//pBtCoexist->fBtcSetRfReg(pBtCoexist, BTC_RF_A, 0x2, 0xfffff, 0x0);
	halbtc8192e1ant_InitCoexDm(pBtCoexist);
}

VOID
EXhalbtc8192e1ant_Periodical(
	IN	PBTC_COEXIST			pBtCoexist
	)
{
	static u1Byte		disVerInfoCnt=0;
	u4Byte				fwVer=0, btPatchVer=0;
	PBTC_BOARD_INFO		pBoardInfo=&pBtCoexist->boardInfo;
	PBTC_STACK_INFO		pStackInfo=&pBtCoexist->stackInfo;

	RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ==========================Periodical===========================\n"));

	if(disVerInfoCnt <= 5)
	{
		disVerInfoCnt += 1;
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ****************************************************************\n"));
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], Ant PG Num/ Ant Mech/ Ant Pos = %d/ %d/ %d\n",
			pBoardInfo->pgAntNum, pBoardInfo->btdmAntNum, pBoardInfo->btdmAntPos));
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], BT stack/ hci ext ver = %s / %d\n",
			((pStackInfo->bProfileNotified)? "Yes":"No"), pStackInfo->hciVersion));
		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_BT_PATCH_VER, &btPatchVer);
		pBtCoexist->fBtcGet(pBtCoexist, BTC_GET_U4_WIFI_FW_VER, &fwVer);
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], CoexVer/ FwVer/ PatchVer = %d_%x/ 0x%x/ 0x%x(%d)\n",
			GLCoexVerDate8192e1Ant, GLCoexVer8192e1Ant, fwVer, btPatchVer, btPatchVer));
		RT_TRACE(COMP_COEX, DBG_LOUD, ("[BTCoex], ****************************************************************\n"));
	}

#if(BT_AUTO_REPORT_ONLY_8192E_1ANT == 0)
	halbtc8192e1ant_QueryBtInfo(pBtCoexist);
	halbtc8192e1ant_MonitorBtEnableDisable(pBtCoexist);
#else
	halbtc8192e1ant_MonitorBtCtr(pBtCoexist);
	halbtc8192e1ant_MonitorWiFiCtr(pBtCoexist);

	if( halbtc8192e1ant_IsWifiStatusChanged(pBtCoexist) ||
		pCoexDm->bAutoTdmaAdjust )
	{

			halbtc8192e1ant_RunCoexistMechanism(pBtCoexist);
	}

	pCoexSta->specialPktPeriodCnt++;
#endif
}


VOID
EXhalbtc8192e1ant_DbgControl(
	IN	PBTC_COEXIST			pBtCoexist,
	IN	u1Byte				opCode,
	IN	u1Byte				opLen,
	IN	pu1Byte				pData
	)
{
	switch(opCode)
	{
		case BTC_DBG_SET_COEX_NORMAL:
			pBtCoexist->bManualControl = FALSE;
			halbtc8192e1ant_InitCoexDm(pBtCoexist);
			break;
		case BTC_DBG_SET_COEX_WIFI_ONLY:
			pBtCoexist->bManualControl = TRUE;
			halbtc8192e1ant_PowerSaveState(pBtCoexist, BTC_PS_WIFI_NATIVE, 0x0, 0x0);
			halbtc8192e1ant_PsTdma(pBtCoexist, NORMAL_EXEC, FALSE, 9);
			break;
		case BTC_DBG_SET_COEX_BT_ONLY:
			// todo
			break;
		default:
			break;
	}
}

#endif


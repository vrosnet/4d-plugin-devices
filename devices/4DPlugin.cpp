/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : Devices
 #	author : miyako
 #	2015/04/28
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
// --- Devices

		case 1 :
			DEVICES_LIST(pResult, pParams);
			break;

	}
}

// ------------------------------------ Devices -----------------------------------


void DEVICES_LIST(sLONG_PTR *pResult, PackagePtr pParams)
{
	ARRAY_TEXT Param1;
	ARRAY_TEXT Param2;
	ARRAY_TEXT Param3;
	ARRAY_TEXT Param4;
	ARRAY_TEXT Param5;
	ARRAY_TEXT Param6;
    
    Param1.setSize(1);
    Param2.setSize(1);
    Param3.setSize(1);
    Param4.setSize(1);
    Param5.setSize(1);
    Param6.setSize(1);
                
#if VERSIONMAC

    NSArray *devices = [DRDevice devices];
    
    for (unsigned int i = 0; i < [devices count];++i){
    
        DRDevice *device = [devices objectAtIndex:i];
        Param1.appendUTF16String([[devices objectAtIndex:i] displayName]);
                
        NSDictionary *info = [device info];
        Param2.appendUTF16String([NSString stringWithFormat:@"%@", [info objectForKey:DRDevicePhysicalInterconnectKey]]);
        Param3.appendUTF16String([NSString stringWithFormat:@"%@", [info objectForKey:DRDeviceVendorNameKey]]);
        Param4.appendUTF16String([NSString stringWithFormat:@"%@", [info objectForKey:DRDeviceProductNameKey]]);
        
        Param5.appendUTF16String([NSString stringWithFormat:@"%d", [device writesCD]]);
        Param6.appendUTF16String([NSString stringWithFormat:@"%d", [device writesDVD]]);
                        
    }

#else
    DWORD len = GetLogicalDriveStrings(0, NULL);
    if(len){
    
        std::vector<wchar_t> buf(len + 1);
        GetLogicalDriveStrings(len, (LPTSTR)&buf[0]);
    
        HANDLE hDevice;
        
        UCHAR lpOutBuffer[2048];
        DWORD nOutBufferSize = sizeof(lpOutBuffer);
        ULONG lpBytesReturned;
		BOOL bResult; 

        for (LPTSTR lpDrive = (LPTSTR)&buf[0]; *lpDrive != 0; lpDrive += 4){
          
            std::wstring drv = L"\\\\.\\";
			
			drv += lpDrive;
            drv = drv.erase(6);
            
            hDevice = INVALID_HANDLE_VALUE;
            
            hDevice = CreateFileW(drv.c_str(), 
                        GENERIC_READ,             
                        FILE_SHARE_READ |   
                        FILE_SHARE_WRITE, 
                        NULL,               
                        OPEN_EXISTING,      
                        0,                  
                        NULL);             
                        
            if (hDevice != INVALID_HANDLE_VALUE){
            
                bResult = DeviceIoControl(hDevice,              
                            IOCTL_STORAGE_GET_MEDIA_TYPES_EX,   
                            NULL, 0,                        
                            &lpOutBuffer, nOutBufferSize, &lpBytesReturned,
                            NULL); 
                
                if (bResult){

                    PGET_MEDIA_TYPES pMediaTypes = (PGET_MEDIA_TYPES) lpOutBuffer;
                    
					if((pMediaTypes->DeviceType == FILE_DEVICE_DVD) || (pMediaTypes->DeviceType == FILE_DEVICE_CD_ROM)){
                        
                        std::wstring drvLetter;
                        drvLetter = lpDrive;
                        drvLetter = drvLetter.substr(0, 2);
                            
						Param1.appendUTF16String((PA_Unichar *)drvLetter.c_str(), drvLetter.size());

						switch(pMediaTypes->DeviceType){
                    
                        case FILE_DEVICE_DVD:
							Param2.appendUTF8String((uint8_t *)"DVD", 3);
                            break;
                        case FILE_DEVICE_CD_ROM:
							Param2.appendUTF8String((uint8_t *)"CD", 2);
                            break;                            
                    
						}
                        
                        //not used on windows
                        Param3.appendUTF8String((uint8_t *)"", 0);
                        Param4.appendUTF8String((uint8_t *)"", 0);
                        Param5.appendUTF8String((uint8_t *)"", 0);
                        Param6.appendUTF8String((uint8_t *)"", 0);                        
                
					}
                    
                }
                
                CloseHandle(hDevice);  
                  
            } 
        
        }
        
    }


#endif

	Param1.toParamAtIndex(pParams, 1);
	Param2.toParamAtIndex(pParams, 2);
	Param3.toParamAtIndex(pParams, 3);
	Param4.toParamAtIndex(pParams, 4);
	Param5.toParamAtIndex(pParams, 5);
	Param6.toParamAtIndex(pParams, 6);    
}


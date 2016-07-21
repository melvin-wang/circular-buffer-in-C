/*
 * License: LGPL      
 *
 * DESCRIPTION:	circular buffer operation functions
 *              
 * REVISION HISTORY:
 *
 * Num      Modified by            Date            Reason
 * ------   -----------------      -------------   -----------------
 * 100      Melvin                 2013-1-14       Created.
 *
 */
#include "buffer.h"
#include <string.h>

typedef struct
{
	int32_t			recordSize;	/*record size*/
	int32_t			recordSum;	/*buffer size*/
	int32_t			out;		/*read pointer*/
	int32_t			in;			/*write pointer*/
	char			*pData;		/*data buffer*/
	
}BUFFER_DATA;

/*��ʼ��Buffer��recordSizeΪÿ����¼�Ĵ�С��recordSumΪ��¼����*/
int32_t bufferInit(BufferHandle* pBufferHandle, int32_t recordSize, int32_t recordSum)
{
	BUFFER_DATA 	*ptr = NULL;
	int32_t			size = sizeof(BUFFER_DATA) + (recordSum + 1) * recordSize;

	ptr = (BUFFER_DATA*)calloc(1, size);
	if(ptr == NULL)
	{
		return BUFFER_MALLOC_FAIL;
	}
	
	ptr->pData = (char*)(ptr + 1);

	ptr->out = ptr->in = 0;
	ptr->recordSum = recordSum + 1;
	ptr->recordSize = recordSize;
	
	*((BUFFER_DATA**)pBufferHandle) = ptr;

	return BUFFER_SUCCESS;
}

/*�ͷ�Buffer��ռ�ڴ�ռ�*/
void bufferFree(BufferHandle bufferHandle)
{
	BUFFER_DATA *ptr = (BUFFER_DATA*)bufferHandle;
	
	if(ptr != NULL)
	{
		free(ptr);
	}
}

/*���Buffer����������*/
inline void bufferClear(BufferHandle bufferHandle)
{
	BUFFER_DATA *ptr = (BUFFER_DATA*)bufferHandle;
	ptr->out = ptr->in = 0;
}

/*��Buffer��д��һ����¼*/
inline int32_t bufferWrite(BufferHandle bufferHandle, void* pRecord)
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	
	if(ptr->out == ((ptr->in+1)%ptr->recordSum)) //full
	{
		return BUFFER_IS_FULL;
	}

	memcpy(&(ptr->pData[ptr->recordSize * ptr->in]), pRecord, ptr->recordSize);

	ptr->in = (ptr->in+1) % ptr->recordSum;

	return BUFFER_SUCCESS;
}

/*��Buffer�ж�ȡһ�ڼ�¼������pRecord��ָ����ڴ档��pRecordΪNULL,ֻ�޸�ָ��*/
inline int32_t bufferRead(BufferHandle bufferHandle, void* pRecord) 
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	
	if(ptr->out == ptr->in) //empty
	{
		return BUFFER_IS_EMPTY;
	}

	if(pRecord != NULL)
	{
		memcpy(pRecord, &(ptr->pData[ptr->recordSize * ptr->out]), ptr->recordSize);
	}
	
	ptr->out = (ptr->out+1) % ptr->recordSum;
	return BUFFER_SUCCESS;
}

/*�ж�Buffer�Ƿ�Ϊ��*/
inline int32_t bufferIsEmpty(BufferHandle bufferHandle)
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	return (ptr->out == ptr->in) ? BUFFER_TRUE : BUFFER_FALSE;
}

/*�ж�Buffer�Ƿ�����*/
inline int32_t bufferIsFull(BufferHandle bufferHandle)
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	return (ptr->out == ((ptr->in+1)%ptr->recordSum)) ? BUFFER_TRUE : BUFFER_FALSE;
}

/*����Buffer�м�¼��*/
inline int32_t bufferSize(BufferHandle bufferHandle)
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	return (ptr->recordSum + ptr->in - ptr->out) % ptr->recordSum;
}

/*�鿴ָ����ŵļ�¼,���޸Ķ�дָ��,��¼��ŷ�ΧΪ[0, size-1]*/
inline void* bufferPeer(BufferHandle bufferHandle, int32_t index)
{
	BUFFER_DATA *ptr = (BUFFER_DATA *)bufferHandle;
	if((index < 0) || (index >= bufferSize(bufferHandle)))
	{
		return NULL;
	}
	
	return &(ptr->pData[ptr->recordSize * ((index + ptr->out) % ptr->recordSum)]);
}


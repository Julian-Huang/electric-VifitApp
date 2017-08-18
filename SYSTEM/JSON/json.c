#include "json.h"



//void HeightWeight()
//{
//	json_t *root;
//	char *out;
//	root = json_pack("{s:b,s:b,s:i,s:i}","hasPerson",1,"unstable",1,"weight",USART2_RX_BUF[USART2_RX_STA],"height",USART2_RX_BUF[USART2_RX_STA+1]);
//}

//void ReadJsonData(void)
//{
//	int value1 = 0;
//	const char *value2 = NULL;
//	const char *value3 = NULL;
//	const char *value4 = NULL;
//	const char *value5 = NULL;
//	const char *value0 = NULL;
//	const char *Json = "[ \
//	{	\
//		\"object1-key1\": 123,	\
//			\"object1-key2\": \"abc\"	\
//	},	\
//	{	\
//		\"test\": \"x\",	\
//			\"test2\": \"x2\"	\
//		},	\
//		{	\
//			\"key\": \"y\"	\
//		}	\
//]" ;
//	json_error_t error;
//	int iRet;
//	json_t *root = json_loads(Json,JSON_REJECT_DUPLICATES,&error);
//	if(root == NULL)
//	{
//		  printf("%s",error.text);
////		  return -1;
//	
//	}
//	printf("%s\n",Json);

//	///first solution to analyze quickly when u know keys.
//  iRet = json_unpack(root, "[{s:i,s:s},{s:s,s:s},{s:s}]", "object1-key1", &value1, 
//		"object1-key2", &value2,
//		"test", &value3,
//		"test2",&value4,
//		"key", &value5);
//	
////	assert( iRet == 0 );
//	json_decref(root);
//	printf("%d\n",iRet);
//	printf("%d\n", value1);
//	printf("%s\n", value2);
//	printf("%s\n", value3);
//	printf("%s\n", value4);
//	printf("%s\n", value5);
////	return 0;
//}


void jansson_pack_test(json_t *text)
{
	char *out;
	
	out = json_dumps(text,JSON_ENCODE_ANY);
	
	printf("%s\r\n",	out);
}


char *jansson_unpack_test(json_t *text)
{

	char *CMD;
	
	if(json_unpack(text, "{ss}", "status",&CMD))
		printf("error\n");
	
	return CMD;
}

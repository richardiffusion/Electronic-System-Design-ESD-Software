#if !defined(AFX_FUNCTION_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
#define AFX_FUNCTION_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_

void ff_to_str(int x, char *string);
void mil_to_str(int x, char *string);
//void memory_error(void);
int Divide(char *string, char s123[10][40]);
int Divide_l(char *string, char s123[20][200]);
int Divide_long(char *string, char s123[200][40]);
int Divide_Recode(char *string, char s123[100][100]);
int Divide_Track(char *string, char *s1, char *s2);
int partstrcmp( char *libstr, char *compstr);
int Stringinvalid(int x,int y,int, int num, int alpha);
int Pad_Pad_Connect(struct Junction *jp1,struct Junction *jp2,int drc);
int Find_min(int main1, short int *distbuf, char *juncbuf, int num);
int Line_Line_Connect(struct Line *oneline1,struct Line *oneline2, int drc);
int Line_Pad_Connect(struct Line *oneline,struct Junction *jp, int drc);
int Line_Arc_Connect(struct Line *oneline, struct Arc *onearc,int drc);
int Line_Fill_Connect(struct Line *oneline,struct Fill *onefill, int drc);
int Pad_Arc_Connect(struct Junction *jp, struct Arc *onearc, int drc);
int Pad_Fill_Connect(struct Junction *onejunc, struct Fill *onefill, int drc);
int Fill_Fill_Connect(struct Fill *fill1,struct Fill *fill2, int drc);














#endif // !defined(AFX_FUNCTION_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
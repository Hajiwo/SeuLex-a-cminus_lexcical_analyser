
const int listsize=50000;
char Readlist[listsize*2];//
int act;
int sBegin=-1;//
int sEnd=-1;//

int yyLex(ifstream&);
int tokenType(int);

int yyLex(ifstream& file){
     if(sBegin==-1){
        char s;
        for(int i(0);i<listsize*2;i++){
          if(file.get(s)){
                Readlist[i]=s;
          }else{
                Readlist[i]=0;
          } 
          //if(Readlist[i]!=0)
          //cout<<Readlist[i];
        }
        sBegin=sEnd=0;
      
     }

     act = 0;
     int mid=sBegin;
     sEnd = sBegin;
     lastState = presentState=0;
      if(Readlist[sEnd]==0) return 0;
     //cout<<"(begin,end)=("<<sBegin<<" ,";

     while(1){
       presentState = Dtran[lastState][int(Readlist[sEnd])];
       if( presentState!=-1 ){ 
         if(action.count(presentState)>0) {
         mid = sEnd;
         act = presentState;
          }
        sEnd++;
        sEnd%=(listsize*2);
        lastState = presentState;
        continue; 
        }else{
          yytext.clear();
          for(int k(sBegin);k<=mid;k++){
            yytext.push_back(Readlist[k]);
          }
           if(sBegin<listsize&&mid>=listsize-1){
              for(int j(0);j<listsize;j++){
                char s;
                if(file.get(s)){
                Readlist[j]=s;
               }else{
                Readlist[j]=0;
                    } 
              }
           }else if(sBegin>=listsize&&(mid+1)%(2*listsize)<listsize){
                for(int j(listsize);j<2*listsize;j++){
                char s;
                if(file.get(s)){
                Readlist[j]=s;
               }else{
                Readlist[j]=0;
                    } 
              }
           }
           sBegin=mid+1;
           //cout<<mid<<")\n";
           break;
        }
     }
  
  return tokenType(act);

}

//intermediate function:
int tokenType(int act){
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define DIMMAX 20

typedef enum {rosso, nero} Colore;
typedef enum {demolita, valida} Statostazione;

typedef struct Stazione{
	unsigned int distanza;
	unsigned int numero_auto;
	unsigned int* automobili;
	unsigned int autonomia_maggiore;
	unsigned int tappe;
	struct Stazione* migliore;
	//1 nero 0 rosso
	Colore colore;
	struct Stazione* destro;
	struct Stazione* sinistro;
	struct Stazione* genitore;
	Statostazione stato;
}Stazione;

Stazione* radice_stazioni=NULL;
unsigned int partenzaP;
unsigned int arrivoP;

void rotazione_a_destra(Stazione* temp){
    Stazione* sinistro=temp->sinistro;
    temp->sinistro=sinistro->destro;
    if(temp->sinistro!=NULL){
    	temp->sinistro->genitore=temp;
	}
    sinistro->genitore=temp->genitore;
    if(temp->genitore==NULL){
    	radice_stazioni=sinistro;
	}else if(temp==temp->genitore->sinistro){
		temp->genitore->sinistro=sinistro;
	}else{
		temp->genitore->destro=sinistro;
	}
    sinistro->destro=temp;
    temp->genitore=sinistro;
}
 
void rotazione_a_sinistra(Stazione* temp){
    Stazione* destro=temp->destro;
    temp->destro=destro->sinistro;
    if(temp->destro!=NULL){
    	temp->destro->genitore=temp;
	}
    destro->genitore=temp->genitore;
    if(temp->genitore==NULL){
    	radice_stazioni=destro;
	}else if(temp==temp->genitore->sinistro){
    	temp->genitore->sinistro=destro;
	}else{
		temp->genitore->destro=destro;
	}
    destro->sinistro=temp;
    temp->genitore=destro;
}

void sistema_inserimento(Stazione* staz) {
	Stazione* zio;
    while(staz!=radice_stazioni&&staz->genitore->colore==rosso){
        if(staz->genitore==staz->genitore->genitore->sinistro){
            zio=staz->genitore->genitore->destro;
            if(zio!=NULL&&zio->colore==rosso){
                staz->genitore->colore=nero;
                zio->colore=nero;
                staz->genitore->genitore->colore=rosso;
                staz=staz->genitore->genitore;
            }else{
                if(staz==staz->genitore->destro){
                    staz=staz->genitore;
                    rotazione_a_sinistra(staz);
                }
                staz->genitore->colore=nero;
                staz->genitore->genitore->colore=rosso;
                rotazione_a_destra(staz->genitore->genitore);
            }
        }else{
            zio=staz->genitore->genitore->sinistro;
            if(zio!=NULL&&zio->colore==rosso) {
                staz->genitore->colore=nero;
                zio->colore=nero;
                staz->genitore->genitore->colore=rosso;
                staz=staz->genitore->genitore;
            }else{
                if(staz==staz->genitore->sinistro){
                    staz=staz->genitore;
                    rotazione_a_destra(staz);
                }
                staz->genitore->colore=nero;
                staz->genitore->genitore->colore=rosso;
                rotazione_a_sinistra(staz->genitore->genitore);
            }
        }
    }
    radice_stazioni->colore=nero;
}

void inserisci(int distanza, int numero_auto){
	unsigned int autonomia;
	Stazione* gen=NULL;
	Stazione* temp=radice_stazioni;
	Stazione* s=(Stazione*)malloc(sizeof(Stazione));
	s->distanza=distanza;
	s->numero_auto=numero_auto;
	s->autonomia_maggiore=0;
	s->automobili=NULL;
	s->tappe=0;
	s->migliore=NULL;
	s->stato=valida;
	if(numero_auto>0){
		s->automobili=(unsigned int*)malloc(sizeof(unsigned int)*numero_auto);
		short i;
		for(i=0;i<numero_auto;i++){
            if(scanf("%d",&autonomia)==1){}
			s->automobili[i]=autonomia;
            if(autonomia>s->autonomia_maggiore){
        		s->autonomia_maggiore=autonomia;
			}
        }
	}
	while(temp!=NULL){
		gen=temp;
		if(s->distanza<temp->distanza){
			temp=temp->sinistro;
		}else{
			temp=temp->destro;
		}
	}
	s->genitore=gen;
	if(gen==NULL){
		radice_stazioni=s;
		s->sinistro=NULL;
		s->destro=NULL;
		s->colore=nero;
		return;
	}else if(s->distanza<gen->distanza){
		gen->sinistro=s;
	}else{
		gen->destro=s;
	}
	s->sinistro=NULL;
	s->destro=NULL;
	s->colore=rosso;
	sistema_inserimento(s);
}

Stazione* minimo(Stazione* s){
	if(s==NULL){
		return NULL;
	}
	while(s->sinistro!=NULL){
		s=s->sinistro;
	}
	return s;	
}

Stazione* massimo(Stazione* s){
	if(s==NULL){
		return NULL;
	}
	while(s->destro!=NULL){
		s=s->destro;
	}
	return s;	
}

Stazione* successore(Stazione* s){
	if(s->destro!=NULL){
		return minimo(s->destro);
	}
	Stazione* gen=s->genitore;
	while(gen!=NULL&&s==gen->destro){
		s=gen;
		gen=gen->genitore;
	}
	return gen;
}

Stazione* predecessore(Stazione* s){
	if(s->sinistro!=NULL){
		return massimo(s->sinistro);
	}
	Stazione* gen=s->genitore;
	while(gen!=NULL&&s==gen->sinistro){
		s=gen;
		gen=gen->genitore;
	}
	return gen;
}

Stazione* ricerca(int distanza){
	Stazione* s;
	s=radice_stazioni;
	while(s!=NULL&&s->distanza!=distanza){
		if(s->distanza<distanza){
			s=s->destro;
		}else{
			s=s->sinistro;
		}
	}
	return s;	
}

void aggiungi_stazione(){
    unsigned int distanza;
    unsigned int numero_auto;
    Stazione* s;
    if(scanf("%d",&distanza)==1){}
    s=ricerca(distanza);
    if(s!=NULL&&s->stato==valida){
		printf("non aggiunta\n");
        return;
	}else if(s!=NULL&&s->stato==demolita){
		unsigned int autonomia;
		if(scanf("%d",&numero_auto)==1){}
		s->numero_auto=numero_auto;
		s->autonomia_maggiore=0;
		s->automobili=NULL;
		s->tappe=0;
		s->migliore=NULL;
		s->stato=valida;
		if(numero_auto>0){
			s->automobili=(unsigned int*)malloc(sizeof(unsigned int)*numero_auto);
			short i;
			for(i=0;i<numero_auto;i++){
	            if(scanf("%d",&autonomia)==1){}
				s->automobili[i]=autonomia;
	            if(autonomia>s->autonomia_maggiore){
	        		s->autonomia_maggiore=autonomia;
				}
	        }
		}
		printf("aggiunta\n");
		return;
	}
	if(scanf("%d",&numero_auto)==1){}
	inserisci(distanza,numero_auto); 
	printf("aggiunta\n");
}

void aggiungi_auto(){
	unsigned int distanza;
	unsigned int autonomia;
	Stazione* s1;
	if(scanf("%d",&distanza)==1){}
	s1=ricerca(distanza);
	if(s1==NULL){
		printf("non aggiunta\n");
        return;
	}else if(s1!=NULL&&s1->stato==demolita){
		printf("non aggiunta\n");
        return;
	}
    if(scanf("%d",&autonomia)==1){}
    unsigned int* nuova_auto;
	nuova_auto=(unsigned int*)realloc(s1->automobili, sizeof(unsigned int)*(s1->numero_auto+1));
    if(nuova_auto!=NULL){
    	s1->automobili=nuova_auto;
	}
	s1->automobili[s1->numero_auto]=autonomia;
    if(autonomia>s1->autonomia_maggiore){
        s1->autonomia_maggiore=autonomia;
	}
	s1->numero_auto++;
    printf("aggiunta\n");
}

void rottama_auto(){
	unsigned int distanza;
	unsigned int autonomia;
	Stazione* s1;
	bool r=0;
	short i;
	if(scanf("%d",&distanza)==1){}
	s1=ricerca(distanza);
	if(s1==NULL){
		printf("non rottamata\n");
        return;
	}else if(s1!=NULL&&s1->stato==demolita){
		printf("non rottamata\n");
        return;
	}
    if(scanf("%d",&autonomia)==1){}
    unsigned int* nuova_auto;
	if(s1->autonomia_maggiore==autonomia){
		s1->autonomia_maggiore=0;
		r=1;
	}
	bool ok=0;
	for(i=0;i<s1->numero_auto;i++){
		if(s1->automobili[i]==autonomia&&ok==0){
			s1->automobili[i]=s1->automobili[s1->numero_auto-1];
			s1->automobili[s1->numero_auto-1]=0;
			printf("rottamata\n");
			ok=1;
		}
	}
	if(i==s1->numero_auto&&ok==0){
		printf("non rottamata\n");
		return;
	}
	s1->numero_auto--;
	nuova_auto=(unsigned int*)realloc(s1->automobili, sizeof(unsigned int)*(s1->numero_auto));
    if(nuova_auto!=NULL){
    	s1->automobili=nuova_auto;
	}
	if(r==1){
		for(i=0;i<s1->numero_auto;i++){
			if(s1->automobili[i]>s1->autonomia_maggiore){
				s1->autonomia_maggiore=s1->automobili[i];
			}	
		}
	}
}

void demolisci_stazione(){
	unsigned int distanza;
	if(scanf("%d",&distanza)==1){}
	Stazione* s1;
	s1=ricerca(distanza);
	if(s1==NULL){
		printf("non demolita\n");
        return;
	}else if(s1!=NULL&&s1->stato==demolita){
		printf("non demolita\n");
        return;
	}
	free(s1->automobili);
	s1->stato=demolita;
	printf("demolita\n");
    return;
}

void NPDX(Stazione* s1){
	s1=predecessore(s1);
	Stazione* s2;
	while(s1!=NULL&&s1->distanza>=partenzaP){
		if(s1->distanza+s1->autonomia_maggiore>=arrivoP&&s1->stato==valida){
			s1->migliore=ricerca(arrivoP);
			s1->tappe=2;		
		}else if(s1->stato==valida){
			s2=successore(s1);
			s1->tappe=0;
			while(s2!=NULL&&s2->distanza<arrivoP&&s1->distanza+s1->autonomia_maggiore>=s2->distanza){
				if(s1->tappe==0&&s2->tappe!=0){
					s1->tappe=s2->tappe+1;
					s1->migliore=s2;
				}else if(s1->tappe!=0&&s2->tappe!=0&&s1->tappe>s2->tappe+1){
					s1->tappe=s2->tappe+1;
					s1->migliore=s2;
				}
				s2=successore(s2);
			}
		}
		s1=predecessore(s1);
	}
}

void NPSX(Stazione* s1){
	s1=successore(s1);
	Stazione* s2;
	int pos;
	while(s1!=NULL&&s1->distanza<=partenzaP){
		if(s1->stato==valida){
			pos=s1->distanza-s1->autonomia_maggiore;
			if(pos<=0){
				pos=0;
			}
		}
		if(pos<=arrivoP&&s1->stato==valida){
			s1->migliore=ricerca(arrivoP);
			s1->tappe=2;		
		}else if(s1->stato==valida){
			s2=predecessore(s1);
			s1->tappe=0;
			pos=s1->distanza-s1->autonomia_maggiore;
			while(s2!=NULL&&s2->distanza>arrivoP&&pos<=s2->distanza){
				if(s1->tappe==0&&s2->tappe!=0){
					s1->tappe=s2->tappe+1;
					s1->migliore=s2;
				}else if(s1->tappe!=0&&s2->tappe!=0&&s1->tappe>s2->tappe+1){
					s1->tappe=s2->tappe+1;
					s1->migliore=s2;
				}else if(s1->tappe!=0&&s2->tappe!=0&&s1->tappe==s2->tappe+1){
					Stazione* jolly1;
					Stazione* jolly2;
					jolly1=s1->migliore;
					jolly2=s2;
					while(jolly1->migliore!=jolly2->migliore){
						jolly1=jolly1->migliore;
						jolly2=jolly2->migliore;
					}
					if(jolly1->distanza>jolly2->distanza){
						s1->tappe=s2->tappe+1;
						s1->migliore=s2;
					}
				}
				s2=predecessore(s2);
			}
		}
		s1=successore(s1);
	}
}

void stampa_percorso(Stazione *s1){
	if(s1->tappe==0){
		printf("nessun percorso\n");
		return;
	}
	printf("%d ",s1->distanza);
	while(s1!=NULL&&s1->distanza!=arrivoP){
		printf("%d ",s1->migliore->distanza);
		s1=s1->migliore;
	}
	printf("\n");
}

void azzera_tappe(Stazione* sp){
	if(partenzaP<arrivoP){
		while(sp!=NULL&&sp->distanza<=arrivoP){
			sp->tappe=0;
			sp->migliore=NULL;
			sp=successore(sp);
		}
	}else{
		while(sp!=NULL&&sp->distanza>=arrivoP){
			sp->tappe=0;
			sp->migliore=NULL;
			sp=predecessore(sp);
		}
	}
}

int main(){
	char comando[DIMMAX];
	radice_stazioni=NULL;
    Stazione* s1;
    Stazione *p;
	while(scanf("%s",comando)!=EOF){    
		if(strcmp("aggiungi-stazione",comando)==0) aggiungi_stazione();
        if(strcmp("demolisci-stazione",comando)==0) demolisci_stazione();
        if(strcmp("aggiungi-auto",comando)==0) aggiungi_auto();
        if(strcmp("rottama-auto",comando)==0) rottama_auto();
		if(strcmp("pianifica-percorso",comando)==0){
			if(scanf("%d",&partenzaP)==1){}
			if(scanf("%d",&arrivoP)==1){}
			if(partenzaP==arrivoP){
				printf("%d\n",partenzaP);
			}else{
				p=ricerca(partenzaP);
				if(partenzaP<arrivoP){
					if(partenzaP+p->autonomia_maggiore>=arrivoP){
						printf("%d %d\n",partenzaP, arrivoP);
					}else{
						s1=ricerca(arrivoP);
						NPDX(s1);
						stampa_percorso(p);
						azzera_tappe(p);
					}
				}else{
					int pos=partenzaP-p->autonomia_maggiore;
					if(pos<=0){
						pos=0;
					}
					if(pos<=arrivoP){
						printf("%d %d\n",partenzaP, arrivoP);
					}else{
						s1=ricerca(arrivoP);
						NPSX(s1);
						stampa_percorso(p);
						azzera_tappe(p);
					}
				}
			}
		}
	}
	return 0;
}

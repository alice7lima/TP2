#include <bits/stdc++.h>
#include <fstream>
#include <functional>
#include <any>

using namespace std::placeholders;

typedef std::function<std::string(std::string)> str;
typedef std::function<std::vector<std::string>(std::string)> vector;
typedef std::function<std::vector<std::string>(std::vector<std::string>)> vector_str;
typedef std::function<std::vector<std::pair<std::string, int>>(std::vector<std::string>)> pair;
typedef std::function<std::vector<std::pair<std::string, int>>(std::vector<std::pair<std::string, int>>)> vector_pair;

class TFTheOne{
    public:
        TFTheOne(std::any v){  /*Construtor*/
            value = v;
        }

        /*Funcoes bind*/
        void bind(str func){
            value = func(std::any_cast<std::string>(value));
        }
        
        void bind(vector func){
            value = func(std::any_cast<std::string>(value));
        }

        void bind(vector_str func){
            value = func(std::any_cast<std::vector<std::string>>(value));
        }
        
        void bind(pair func){
            value = func(std::any_cast<std::vector<std::string>>(value));
        }

        void bind(vector_pair func){
            value = func(std::any_cast<std::vector<std::pair<std::string, int>>>(value));
        }

        
        void printme(){  /*Metodo que mostra o valor final*/
            std::vector<std::pair<std::string,int>> aux;
            aux = std::any_cast<std::vector<std::pair<std::string, int>>>(value);
            for(int i = 0; (i < aux.size()) && (i < 25); i++){
                std::cout << aux[i].first << " " << aux[i].second << std::endl;
            }       
        }

    private:
        std::any value;

};

std::string read_file(std::string file){    /*Leitura do  arquivo de onde serao contadas as palavras*/
    std::ifstream myfile(file);
    std::string aux_file;
    int cont = 0;

    while(!myfile.eof()){
        std::string aux;
        getline(myfile, aux);
        if(cont == 0) aux_file = aux;
        else aux_file = aux_file + " " + aux;
        cont++;
    }
    
    return aux_file;
    
}

std::string filter_char(std::string aux_file){   /*Trasformando todos os caracteres especiais e acentos em espaco*/
    for(int i = 0; i < aux_file.size(); i++){
        if(!isalnum(aux_file[i])){
            aux_file[i] = 32;
        }
    }
    return aux_file;
} 

std::string normalize(std::string aux_file){  /*Passando todo o texto que temos para letras minusculas*/
    for(int i = 0; i< aux_file.size(); i++){
        aux_file[i] = tolower(aux_file[i]);
    }
    return aux_file;
}

std::vector<std::string> scan(std::string aux_file){  /*Separando o texto em um vetor de palavras*/
    std::vector<std::string> aux;
    std::string aux2;
    char * split; 
    char str[aux_file.size()];

    strcpy(str, aux_file.c_str());
    split = strtok(str, " ,");

    while(split != NULL){
        aux.push_back(split);
        split = strtok(NULL, " ,");
    }
    
    return aux;
}

std::vector<std::string> remove_stop_words(std::vector<std::string> list){ /*Acessando o txt de palavras proibidas e comparando com nosso vetor de palavras para retirar da contagem*/
    std::vector<std::string> stop_words;
    std::string aux = "stop_words.txt", aux2, aux3;
    
    aux2 = read_file(aux);
    aux3 = normalize(aux2);
    stop_words = scan(aux3);
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < stop_words.size(); j++){
            if(stop_words[j] == list[i]){
                list.erase(list.begin()+i);
                i--;
            }
        }
    }
    return list;
}

std::vector<std::pair<std::string, int>> frequencies(std::vector<std::string> aux){  /*Contar a frequencia de ocorrencia de cada palavra no texto*/
    std::vector<std::pair<std::string, int>> freq;
    std::vector<bool> visited(aux.size(), false);

    for(int i = 0; i < aux.size(); i++){
        if(!visited[i]){
            int cont = 1;
            visited[i] = true;
            for(int j = 0; j < aux.size(); j++){
                if(aux[i] == aux[j] and !visited[j]){
                    visited[j] = true;
                    cont++;
                }
            }
            freq.push_back(make_pair(aux[i], cont));
        }
    }
    return freq;
}

bool exe(std::pair<std::string, int > x, std::pair<std::string, int> y){
    if(x.second == y.second){
        return (x.first < y.first);
    }
    return(x.second > y.second);
}

std::vector<std::pair<std::string, int>> sort_(std::vector<std::pair<std::string,int>> freq_words){ /*Ordenar as palavras por frequencia, se a frequencia for igual elas sao ordenadas por ordem alfabetica*/
    std::sort(freq_words.begin(), freq_words.end(), exe);
    return freq_words;
}

int main(int argc, char* argv[]){
    std::string x = "wordcount.txt";

    TFTheOne TFT(x);    /*Instanciando um objeto TheOne*/
    TFT.bind(read_file);
    TFT.bind(filter_char);
    TFT.bind(normalize);
    TFT.bind(scan);
    TFT.bind(remove_stop_words);
    TFT.bind(frequencies);
    TFT.bind(sort_);

    TFT.printme();

    return 0;
}
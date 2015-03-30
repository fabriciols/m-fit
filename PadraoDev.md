# Padrões de Desenvolvimento #

Este documento deverá ser utilizado como base para os padrões e regras de desenvolvimento do MFIT.

1 - Toda classe tem as iniciais Maiúsculas
Exemplo: MinhaClasse e não minhaclasse ou minhaClasse.

2 - Nome de atributos com as primeiras letras minúsculas
Exemplo: meuAtributo

3 - Variáveis Locais segue o mesmo padrão dos atributos

4 - Qualquer função/atributo/qquer coisa em INGLÊS !
Exemplo: Data e não Dado; Date e não Data

5 - Identação de funções
Padrão VI (:) :
```
int main(void)
{
   int a, int b;
   return;
}
```

6 - Espaçamento: return (maria) e não return ( maria );

7 - Ifs:
TODO IF deve conter os {}
Exemplo (Errado):
```
if (maria)
   return (2);
```

Exemplo (Certo)
```
if (maria)
{
   return (2);
}
```

8 - Comentário
Padrão:

```
/************************************************************************
* Função que faz X Y e depois Z
*************************************************************************
* param (E): param1  - representa XYZ
* param (S): param2  - representa QWER
*************************************************************************
* return   : int     - retorna 0 qndo X e 1 qndo Y
*************************************************************************
* Histórico:
* 26/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
```
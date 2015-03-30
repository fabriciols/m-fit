
```
   Filtragem Espacial é uma técnica utilizada na área de processamento de imagens para 
realizar a suavização ou realce de detalhes da imagem, uma detecção de bordas ou até mesmo
 a eliminação de elementos indesejáveis dessas imagens, como por exemplo, os ruídos. Para 
a aplicação de um filtro, utiliza-se o conceito de operadores de vizinhança, ou seja, 
alterar o Nível Digital (ND) de um pixel através do ND de seus pixels vizinhos. Dependendo
 do filtro a ser utilizado este cálculo pode ser alterado, em geral, é criada uma máscara 
de convolução, que nada mais é do que uma matriz (normalmente de tamanho 3x3) que possui 
um determinado peso em cada uma de suas posições. 
```
> | 1 | 1 | 1 |
|:--|:--|:--|
> | 1 | 8 | 1 |
> | 1 | 1 | 1 |

```
Matriz 3x3 para um filtro passa-alta.

   O pixel a ter seu ND recalculado é sempre aquele que estiver sob a posição central da 
matriz, como mostrado a seguir:

*IMAGEM*
 
Fig 2 – Máscara 3x3 sendo utilizada para a filtragem de uma imagem digital. No caso, o ND 
a ser recalculado é do pixel X.

    Durante a filtragem, esta matriz percorre por toda a imagem pixel a pixel, partindo da 
esquerda para a direita, tal deslocamento é obtido através da soma de 1 pixel na posição 
atual da matriz, como pode ser observado na Fig.2. Entre os principais tipos de filtragem,
 estão o filtro passa-alta, que pode ser uma boa alternativa para fazer a detecção de 
bordas e o filtro passa-baixa, que serve para fazer a suavização da imagem, tornando-a 
mais homogênea, ou seja, há uma diminuição dos possíveis ruídos encontrados na imagem.

 
*IMAGEM*

Fig. 3 – (a) Imagem original, (b) imagem após filtragem passa-baixa, (c) imagem após 
filtragem passa-alta
```
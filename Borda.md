
```

   Detecção de Bordas é uma técnica para localizar os pontos, linhas que dividem regiões 
distintas de uma determinada imagem. Uma borda pode ser definida como uma região da imagem
 que faz a diferenciação entre um objeto e o fundo da imagem. Computacionalmente, 
define-se uma borda como sendo uma região que possui um gradiente de magnitude alta. A 
detecção das bordas pode ser feita através do uso de diversos operadores, tais como o de 
Roberts e o de Sobel, ou simplesmente pela aplicação de um filtro passa-alta. 
   O resultado da operação é totalmente influenciado pela escolha do operador a ser 
aplicado. No caso de um filtro passa-alta, a filtragem se dá através do processo de 
convolução e quando aplicados em regiões homogêneas os valores de ND dos pixels 
encontrados são sempre 0 ou próximos de 0, já no caso de regiões heterogêneas (bordas), o 
valor obtido será alto demais ou baixo demais, tornando este pixel um pixel possivelmente 
pertencente a uma borda, porém este método não é capaz de mostrar qual a direção da borda 
pelo fato de sua detecção ser feita ponto a ponto. 
   O operador de Sobel trabalha em cima dos tons de cinza das imagens e faz uso de duas 
máscaras 3x3, uma para cálculos de gradiente em x e outra em y, além disso, este operador 
utiliza uma técnica de binarização da imagem. Os resultados obtidos pela aplicação deste 
operador são melhores se comparados àqueles obtidos pela filtragem passa-alta e pelo 
operador de Roberts pelo fato de demonstrarem qual a magnitude e direção dos gradientes de
 cada pixel, além de ser menos sensível a ruídos, fazendo com que a imagem gerada tenha 
apenas suas bordas com um realce maior do que na imagem original.

*FIGURA DOS PEIXE*

   Observa-se que na Fig.5a, a imagem ao passar por um filtro-passa alta tem suas bordas 
detectadas de forma pontilhada pelo fato do filtro calcular o valor digital dos pixels de 
forma ponto a ponto, bordas descontínuas, enquanto na Fig. 5b, os traços são contínuos e 
seus resultados (Gradiente) informam magnitude e direção do gradiente. 
   Através do uso das técnicas de Filtragem Espacial, poderemos fazer a segmentação da 
imagem que represente o Ritmo Visual do vídeo através dos filtros passa-baixa para uma 
diminuição de ruídos e homogeneização das imagens e logo após aplicar um filtro passa-alta
 ou um filtro que utilize o operador de Sobel, ficando em evidência maior as linhas que 
nos mostrem onde ficam os pontos de transições do vídeo. 


```
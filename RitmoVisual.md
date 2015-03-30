
```
   Ritmo Visual pode ser dito como sendo a representação de um segmento de imagens 2D (um 
vídeo) em uma única imagem na dimensão 1D, o que torna o trabalho de análise do vídeo em 
um trabalho de segmentação de imagens.
   Existem dois tipos de representações do ritmo visual. Ritmo Visual por histograma e por 
sub-amostragem. Neste trabalho iremos focar mais no ritmo visual por sub-amostragem, o 
qual se caracteriza pela transformação de determinadas faixas de pixels (amostras) de cada
 um dos frames de uma tomada do vídeo em uma coluna da representação 1D, como representado
 na Fig. 5. Podemos dizer então que o Ritmo Visual é uma simplificação do vídeo. 
 
*IMAGEM*

Fig. 5 – Exemplo de transformação da diagonal principal 
dos frames de um vídeo em uma coluna do ritmo visual.


   A representação do vídeo em ritmo visual pode ser obtida através de:
Tentar descrever os passos [olhar ref 17] dessa bosta inclusive com fórmulas!

   O padrão desta representação pode ser alterado conforme a escolha dos pixels, portanto, o 
padrão do ritmo visual é totalmente dependente do tipo da amostragem retirada do vídeo. 
Bons resultados podem ser obtidos se a sub-amostragem do frame for dado pela sua diagonal 
principal, uma vez que ela trará características horizontais e verticais do frame, podendo
 assim fazer uma boa representação do mesmo. Uma melhor abordagem sobre este processo pode
 ser encontrada em [6,17].
   Cada tipo de transição gera uma coluna com características, padrões diferentes no ritmo 
visual, como podem ser observados abaixo:

*IMAGENS* 
(a)(b)(c)

Fig. 6 – Representações de Ritmo Visual - (a) cortes, (b) fade-off seguido de  fade-in, 
(c) Dois dissolves consecutivos. 

   Em nosso trabalho utilizaremos as técnicas de Ritmo Visual para simplificar o nosso 
problema em um problema de segmentação de imagens. A partir daí, faremos uso de outras 
ferramentas de PDI como filtragem espacial e detecção de bordas, juntamente com a 
aplicação de operadores de morfologia matemática, para obter os pontos de transição das 
tomadas.
```
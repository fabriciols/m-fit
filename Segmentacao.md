
```
Finalizando

Em visão computacional, segmentação é o processo de dividr uma imagem
em multiplas regiões , com a intenção de conseguir subdividir partes
importantes da imagem , ou simplesmente modifica-las , ambos
à fim de facilitar uma análise posterior .
Em imagems monocromáticas algoritmos soa baseados em uma das propriedades
basicas de valores de niveis de cinza : descontinuidade(detecção de bordas
e similaridade (agrupamento de regiões homogêneas)

Na descontinuidade o objetivo é particionar a imagem tendo como regra mudanças
bruscas na escala de cinza.

A técnica de similaridade funciona da seguinte maneira : Uma imagem f(x,y) produz
como saida uma imagem g(x,y) , onde cada pixel da imagem f é comparado com um
limiar T , se o nivel de cinza do pixel for maior que o limiar T , este pixe
l é mantido em g , caso contrário ele é descartado .
Esta técnica é amplamente utilizada pois tem um baixo custo computacional

Referências :

http://www.inf.ufrgs.br/pos/SemanaAcademica/Semana98/evandro.html
http://atlas.ucpel.tche.br/~vbastos/segmenta.htm
http://www2.ufpa.br/rcientifica/ed_anteriores/pdf/ed_02_scmn.pdf 
```

# WIKIPEDIA #

```
Em visão computacional, segmentação se refere ao processo de dividir uma imagem digital em 
múltiplas regiões (conjunto de pixels) ou objetos, com o objetivo de simplificar e/ou mudar 
a representação de uma imagem para facilitar a sua análise. Segmentação de imagens é 
tipicamente usada para localizar objetos e formas (linhas, curvas, etc) em imagens.

O resultado da segmentação de imagens é um conjunto de regiões/objetos ou um conjunto de 
contornos extraídos da imagem (ver detecção de borda). Como resultado, cada um dos pixels 
em uma mesma região é similar com referência a alguma característica ou propriedade 
computacional, tais como cor, intensidade, textura ou continuidade. Regiões adjacentes 
devem possuir diferenças significativas com respeito a mesma característica(s).

```
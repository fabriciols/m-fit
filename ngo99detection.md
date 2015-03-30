# Detecção de Transições Graduais através de Análise Temporal de Takes (pedaços) #

## Resumo ##

```
C. W. Ngo et al., 1999, com a intenção de fazer a detecção de transições do tipo corte, 
wipe e dissolve, propuseram um método baseado na análise espaço-temporal de pedaços de um 
vídeo, os quais eram compostos por regiões espacialmente e temporalmente coerentes, os 
frames, este processo consistem em retirar um “pedaço” de cada frame e através de um 
conjunto destes pedaços, fazer uma análise para descobrir qual o tipo de transição que se 
trata. Com a aplicação deste método, foi possível detectar e classificar as transições de 
uma maneira eficiente. Ressalta-se ainda que futuramente, pretende-se fazer uma análise 
dos takes de vídeos para fazer indexação de vídeos e recuperação de dados.  
```

## Tradução ##

```
Resumo

Neste trabalho, apresentamos abordagens para detecção de cortes, wipes e dissolves 
baseadas em analise espaço-temporal de pedaços obtidos de vídeos. Estes pedaços são 
compostos por regiões espacialmente e temporalmente coerentes, que podem ser entendidas 
como frames.  Nos métodos propostos, as pausas da câmera são localizadas através da 
realização da segmentação de cor-textura e análise estatística sobre estas partes de 
vídeo. Além de detectar pausas de câmera, nossos métodos podem classificar as transições 
como cortes, wipes e dissolves de uma maneira eficiente.

Conclusão

Nós propusemos métodos para detecção e classificação de cortes, wipes e dissolves baseados
 em análise de takes de vídeo. Nosso método reduz os problemas de segmentação de vídeo 
para problemas de segmentação de imagem, além de processar frames diretamente no domínio 
MPEG, resultando em um significante ganho de velocidade. Ao contrário da maioria dos 
estudos anteriores que detectam arbitrariamente um frame num wipe ou dissolve como um 
break point, nós detectamos uma seqüência de frames como um break point, daí podendo 
aplicar cálculos de análise de precisão para medir os acertos. Futuramente, nós iremos 
estudar a possibilidade de estimar o movimento e recursos da imagen diretamente de slides 
para indexação de vídeo e recuperação de dados. 

```

## Citação ##

```
C.W. Ngo, T.C. Pong, R.T. Chin, "Detection of Gradual Transitions through Temporal Slice 
Analysis," cvpr, p. 1036,  1999 IEEE Computer Society Conference on Computer Vision and 
Pattern Recognition (CVPR'99) - Volume 1,  1999

```

## Download ##

[ngo99detection.pdf](http://m-fit.googlecode.com/files/ngo99detection.pdf)C.W. Ngo, T.C. Pong, R.T. Chin, "Detection of Gradual Transitions through Temporal Slice 
Analysis," cvpr, p. 1036,  1999 IEEE Computer Society Conference on Computer Vision and 
Pattern Recognition (CVPR'99) - Volume 1,  1999

}}}```
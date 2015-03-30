# Um metodo eficiente para detecção de cortes em uma cena. #

## Resumo ##
```
Com o objetivo de detectar cortes de cenas e otimizar o custo de tempo humano, Jun Yu et
al., 2000, utilizaram a diferença de métrica baseado no histograma de cores de sucessivos
frames de uma sequencia de video utilizando a entropia de um threshould, onde o threshould
é a base da difereça dos histograms de tons de cinza de um determinado frame. Por outro
lado, para confirmar os resultados, são necessárias interações humanas e um procedimento
de eliminação de falsa detecção futuramente será desenvolvido para melhorar a precisão.
```

## Tradução ##

```
Resumo

Em várias aplicações como visualizador de video, indexação de cenas relevantes 
em uma sequencia de video é importante para recuperações. O que geralmente é feito
na indexação de video é identificar um corte representando-o por uma faixa entre
suas cenas. Detecção de cortes em uma cena envolve a identificação dos frames
onde cada conteúdo da cena é significantemente diferente do conteúdo do frame anterior.
Isto requer computação, uma apropriada métrica que caracteriza a mudança de um
conteúdo de video entre dois frames e um inicio para determinar se a largura da mudança
é suficiente para se definir um corte. Com isto, nos propuzemos um inovado método para
detectar cortes de cenas usando a diferença de métrica baseado no histograma de cores de
sucessivos frames de uma sequencia de video. Um método de entropia thresholding é usado
para obter o threshold automaticamente identificando cortes de cenas. Futuramente
aplicaremos o processo de refinamento para remover falsas detecções. Resultados 
experimentais são representados para ilustrar uma boa performance do método.

Conclusão

Detecção de cortes de cenas é o primeiro passo para para amostragens de imagens de videos
temporais uniforme. Com propósito de determinar o frame correspondente para corte, nós
devemos primeiro definir a métrica apropriada que caracteriza a mudança de conteúdo de um
frame, que na pratica é importante, desde a eficiencia da detecção de cortes de cenas
dependendo da robustes no threshold. Para 100% de cortes de cenas corretas, o entendimento
da semantica de um conteudo de video é requirido, através da atual era da arte na
computação visual. Por outro lado, para confirmar os resultados, interações humanas são
necessárias. No mesmo tempo, desde que o banco de dados através da indexação é o ponto
foco para a detecção de cortes, não é preciso ser feito em tempo real. Neste caso
removendo os FAs pela interação humana é factivel. Por outro lado, a principal tarefa para
métodos de corte da cena corrente é tornar a interação humana mais eficiente e diminuir o
tempo trabalho. Em propósito de alcançar estes objetivos, o algoritimo de detecção de
cortes deve ser automatico selecionando o threshold e prover cortes de cenas perdidas
razoavelmente. Com isto, nos propuzemos um inovado método em contrapartida para estes
requisitos visando a detecção de cortes de cenas usando uma diferença de métrica baseado
no histograma de tons de cinza, de sucessivos frames de uma sequencia de video. Um método
de thresholding entrópico é usado para obter o theshould automaticamente para identificar
um corte de uma cena. Uma procedimento de eliminação de falsa detecção futuramente será
desenvolvido para melhorar a precisão. Devido a algumas perdas de cortes e numeros
razoaveis de FAs, com o procedimento de confirmação humana, o processo de detecção da cena
inteira é semi-automática. Estes frames formam a unidade básica para suceder o 
processamento, assim como seleção de frame chave e equiparação de video. Experimentos 
mostram que a performance do método é boa. Por outro lado, é muito útil para aplicações de
banco de dados de video.
```

## Download ##

[An\_efficient\_method\_for\_scene\_cut\_detection.pdf](http://m-fit.googlecode.com/files/An_efficient_method_for_scene_cut_detection.pdf)
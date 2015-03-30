# Segmentação de vídeo baseado em análise de imagens 2D #

## Resumo ##
```
  Silvio J. F. Guimarães et al., 2002, propuseram um método que fazia uso de
Ritmo Visual (transformação dos eventos do vídeo em padrões de imagens 2D),
juntamente com a aplicação de operadores de Morfologia Matemática e
Topologia Digital para fazer a detecção e identificação dos eventos de
transição de um vídeo. Aplicando este método, puderam fazer a detecção de
fades curtos com uma baixa taxa de erros, sendo possível ainda desenvolver
uma extensão do método para detecção de outros tipos de transições, assim
como fazer a aplicação do método diretamente nos dados do vídeo.
```

## Tradução ##

```
Resumo

O problema de segmentação de vídeo consiste na identificação da fronteira entre takes 
consecutivos. A abordagem comum para resolver este problema é baseado em medidas de 
dissimilaridade entre frames. Neste trabalho, a segmentação de vídeo é transformada em um 
problema de detecção de padrões, onde cada evento do vídeo é transformado em um diferente 
padrão em uma imagem 2D, chamada Ritmo Visual, obtendo uma transformação específica. Em 
nossa análise nós usamos ferramentas topológicas e morfológicas para detectar cortes. Além
 disso, nós usamos análise de linhas discretas e análise de árvore máxima para detectar 
transições fade e flashes respectivamente. Nós apresentamos uma análise comparativa do 
nosso método para detecção de cortes em relação a vários outros métodos, que mostram os 
melhores resultados de nosso método.

Discussões e Conclusões

Neste trabalho, nós transformamos o problema de segmentação de vídeo em um problema de 
segmentação de imagens 2D. Métodos para detecção de corte, fade e flash foram propostos. A
 principal contribuição para o nosso trabalho é a aplicação de operadores de morfologia 
matemática e topologia digital para resolver o problema de segmentação de vídeo. A 
exploração do Ritmo Visual por histogramas, principalmente para detecção de fade, também 
representa uma contribuição original para este trabalho. Nós verificamos que nós podemos 
identificar fades curtos com uma pequena proporção de falsas detecções. Para detecção de 
flashes, a extração de picos pela análise de árvore máxima possibilitou a detecção de 
flashes em todas as posições do take.
	Os resultados mais efetivos estão associados com a boa escolha de parâmetros. Dois
 tipos de parâmetros podem ser distinguidos: fixo e variável. Os parâmetros fixos tais 
como o tamanho dos elementos estruturantes, podem ser pré-determinados para todas as 
aplicações. O uso do parâmetro variável, em nosso caso, o valor limiar, é interessante e 
algumas vezes necessário, pois ele desempenha um papel importante no processo de 
segmentação onde o usuário por adequá-lo de acordo com a natureza do dado e o tipo de 
aplicação. Além disso, o ??? deste parâmetro permite chegar a um compromisso entre a 
over-segmentetion e a sub-segmentação.
	Para realizar a análise comparative entre diferentes métodos para detecção de 
eventos, nós definimos quatro medidas de qualidade: robustes, ???, ??? e a medida gama. De
 acordo com estas medidas de qualidade, nós verificamos que o método proposto para 
detecção de corte tem os melhores valores de robustez, ??? e medida gama, quando 
experimentalmente comparado com outros métodos. Nós também computamos as medidas de 
qualidade para detecção de flashes, e de acordo com estas medidas, o método baseado na 
análise de árvore máxima geralmente apresenta os melhores resultados.
	Através deste trabalho, nós observamos que o Ritmo Visual por sub-amostragem e por
 histograma representa uma adequada simplificação do conteúdo do vídeo, que pode 
constituir a base para futuros desenvolvimentos como: identificação de outros eventos de 
vídeo, como pan e zoom para detecção de seus padrões correspondentes; modificar o método 
proposto para detecção de corte para detecção de transições graduais de vídeo, usando o 
gradiente multi-escalar morfológico (Soille, 1999) para computar o gradiente horizontal. 
Nós ainda podemos ressaltar que considerando sequencias de vídeo como imagens 3D, nós 
poderíamos aplicar uma extensão do nosso método diretamente nos dados do vídeo. Nós temos 
que verificar se o cálculo do esforço é recompensado por uma melhor qualidade de 
segmentação. 
```

## Citação ##
```
GUIMARÃES, S. J. F. ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque ; LEITE, Neucimar 
Jerônimo . Video segmentation based on 2D image analysis. Pattern Recognition Letters, v. 
24, p. 947-957, 2003.
```

## Download ##

[Journal\_03\_prl\_jamil.pdf](http://m-fit.googlecode.com/files/Journal_03_prl_jamil.pdf)
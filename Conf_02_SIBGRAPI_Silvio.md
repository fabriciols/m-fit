# Um algoritmo de detecção de transição parametrizada e direcional baseado em resíduos morfológicos #

## Resumo ##

```
Silvio. J. F. Guimarães et al., 2002, com o objetivo de realizar a identificação e 
classificação de transições graduais entre duas zonas planas consecutivas (conjunto de 
pontos consecutivos com os mesmos tons de cinza), aplicaram um método baseado na 
decomposição hierárquica dos componentes da imagem em resíduos morfológicos. Com o uso 
deste método era possível retirar as informações necessárias sobre a transição e ainda 
fazer a reconstrução exata da imagem a partir dos seus resíduos. Desta forma, conseguiram 
fazer a detecção e identificação das transições facilmente. Apesar de o método funcionar 
bem em relação às transições graduais, o trabalho não faz referência às transições bruscas 
(cortes).
```

## Tradução ##

```
Resumo

	A identificação de fronteiras/transições representa um interessante e difícil 
problema no processamento de imagens, principalmente se duas zonas planas estiverem 
separadas por uma transição gradual. O operador mais comum (Soille) funciona muito bem em 
arestas acentuadas, mas falha para transições graduais. Neste trabalho, nós fizemos uma 
caracterização do gradiente através do estudo de abertura de resíduos. Esta 
caracterização é útil para identificar e classificar transições graduais acentuadas entre 
duas zonas planas consecutivas de acordo com um critério de dimensão.

Conclusão

	Neste trabalho, nós propusemos um novo método para caracterizar e identificar a 
direção da transição entre zonas planas consecutivas de acordo com um parâmetro da 
dimensão. Nosso método é baseado na decomposição hierárquica dos componentes da imagem em 
resíduos morfológicos que permitem a análise de suas transições em diferentes níveis de 
decomposição. Segundo a parametrização do nosso método, nós podemos facilmente 
identificar transições de um específico tamanho, enquanto esta identificação não é tão 
fácil em outros métodos, como o gradiente Soille. Além disso, diferentemente do gradiente 
Soille e suas variantes, que consideram cada dimensão separadamente, nosso método 
considera toda a decomposição hierárquica dos dados para obter a informação da transição. 
Isto é possível graças à análise de resíduos morfológicos.
	Uma interessante aplicação para o método proposto aqui concerne no problema da 
segmentação de vídeo em que nós podemos classificar as transições de acordo com a 
parametrização da dimensão. Um exemplo desta aplicação é dada na figura 6 em que nós 
aplicamos o algoritmo proposto em [7,8] para segmentação de vídeo. Neste algoritmo, nós 
podemos substituir o gradiente computacional por nosso algoritmo de detecção de transição 
1D parametrizado.

```

## Citação ##
```
GUIMARÃES, S. J. F. ; LEITE, Neucimar Jerônimo ; COUPRIE, Michel ; ARAÚJO, Arnaldo de 
Albuquerque . A directional and multi-scale method to identify gradual transitions. In: 
XV Brazilian Symposium on Computer Graphics and Image Processing, 2002, Fortaleza. IEEE 
SIBGRAPI 2002, 2002. p. 261-268.  
```

## Download ##

[Conf\_02\_SIBGRAPI\_Silvio.pdf](http://m-fit.googlecode.com/files/Conf_02_SIBGRAPI_Silvio.pdf)
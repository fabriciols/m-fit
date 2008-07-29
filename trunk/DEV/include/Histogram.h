/**
 *Classe responsável por conter
 *os dados do histograma de um frame.
 */
class Histogram 
{
 
	/**
	 *Armazena os dados do histograma
	 *da imagem.
	 */
	public:
		Histogram(IplImage* img);
		double* getMatrix();
		float getMax();
		float getMin();
		int getMaxLuminance();
		int getMinLuminance();


	private:


		double data[255];
	 
	/**
	 *Guarda o maior valor
	 *de luminância do histograma
	 */
		float max; // Nro de pixels de valor maximo
		int maxLum; // Valor da maior luminancia
	 
	/**
	 *Armazena o menor valor
	 *de luminância do histograma
	 */
		float min; // Nro de pixels de menor valor
		int minLum; // Valor da menor luminancia
};

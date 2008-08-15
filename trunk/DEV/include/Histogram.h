/**
 *Classe respons�vel por conter
 *os dados do histograma de um frame.
 */
class Histogram 
{

	private:


		double data[256];
	 
	/**
	 *Guarda o maior valor
	 *de lumin�ncia do histograma
	 */
		float max; // Nro de pixels de valor maximo
		int maxLum; // Valor da maior luminancia
	 
	/**
	 *Armazena o menor valor
	 *de lumin�ncia do histograma
	 */
		float min; // Nro de pixels de menor valor
		int minLum; // Valor da menor luminancia

	/**
	 *Armazena os dados do histograma
	 *da imagem.
	 */

	public:
		
		Histogram();
		Histogram(IplImage* img);
		double* getMatrix();
		float getMax();
		float getMin();
		int getMaxLuminance();
		int getMinLuminance();

};

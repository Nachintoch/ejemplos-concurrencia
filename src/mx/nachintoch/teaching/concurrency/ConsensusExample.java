package mx.nachintoch.teaching.concurrency;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.Random;
import java.util.logging.Logger;
import java.util.logging.Level;

/**
 * Ejemplo de aplicaci&oacute;n de las herramientas que nos proporciona Java
 * para c&oacute;mputo concurrente. En particular, vamos a implementar la
 * soluci&oacute;n al problema del consenso presentada en el libro <i>
 * Synchronization algorithms and concurrent programming</i>, del autor Gadi
 * Taubenfeld, de editorial Pearson Prentice Hall, en las p&aacute;ginas 310 y
 * 311.
 * @author <a href="http://www.nachintoch.mx" >Manuel "Nachintoch" Castillo</a>
 * @version 1.0, november 2015
 */
public class ConsensusExample {

	// atributos de clase

	/**
	 * Registro atr&oacute;mico que contendr&aacute; el resultado de la toma de
	 * decisi&oacute;n.
	 * @since Consensus Example 1.0, november 2015
	 */
    private static AtomicBoolean decision = new AtomicBoolean(false);
    
    /**
	 * Sem&aacute;foro binario que nos va a proveer de exluci&oacute;n mutua
	 * entre los hilos involucrados. Esto nos va a ayudar a simular los bloques
	 * de c&oacute;digo del pseudoc&oacute;digo <i>synchronized</i>.
	 * @since Consensus Example 1.0, november 2015
	 */
    private static Semaphore mutex = new Semaphore(1);
    
    /**
	 * Registro at&oacute;mico que nos ayuda a conocer el estado de la
	 * competencia entre los dos hilos distinguidos que se encargan de tomar la
	 * decisi&oacute;n.
	 * @since Consensus Example 1.0, november 2015
	 */
    private static AtomicBoolean x = new AtomicBoolean(false);
    
    /**
	 * Registro at&oacute;mico que nos ayuda a conocer el estado de la
	 * competencia entre los dos hilos distinguidos que se encargan de tomar la
	 * decisi&oacute;n.
	 * @since Consensus Example 1.0, november 2015
	 */
    private static AtomicBoolean y = new AtomicBoolean(false);
    
    /**
	 * Registro at&oacute;mico que indica al resto de los procesos no
	 * distinguido si se sigue tomando una decisi&oacute;n o si habemus
	 * consensus.
	 * @since Consensus Example 1.0, november 2015
	 */
    private static AtomicBoolean finish = new AtomicBoolean(false);

	// método main

	/**
	 * Instancia tantos procesos como indique el usuario, y los pone a resolver
	 * el problema del consenso con el algoritmo estudiado.
	 * @param args - S&oacute;lo importa que tenga una primer entrada con un
	 * valor entero. Este ser&aacute; el n&uacute;mero de procesos que van a
	 * participar en el problema del consenso.
	 * @throws InterruptedException - Si mientras espera a que terminen los
	 * hilos participantes en el problema del consenso, este hilo es
	 * interrumpido.
	 * @throws ExecutionException - Si hubo un problema al terminar la
	 * ejecuci&oacute;n de los hilos.
	 * @since Consensus Ecample 1.0, november 2015
	 */
    public static void main(String[] args) throws InterruptedException, 
    	ExecutionException {
        Integer procNum = Integer.parseInt(args[0]);
        if(procNum <= 1) {
        	throw new Error("At least, two threads must be created. Use as a "
        		+ "parameter any number greather than 1");
        }//si se pidio crear menos de 2 hilos, chilla
        ExecutorService threadPool = Executors.newFixedThreadPool(procNum);
        Random inputGenerator = new Random();
        Future backToThe = null;
        for(int i = 0; i < procNum; i++) {
            backToThe = threadPool.submit(
            	new Process(i +1, inputGenerator.nextBoolean()));
        }//crea todos los procesos contendientes
        while(!backToThe.isDone()) {
            Thread.sleep(100);
        }//espera a que los procesos terminen de decidir
        System.out.println("Processes decided " +backToThe.get());
        threadPool.shutdown();
    }//main

	// clases anidadas

	/**
	 * Hilo de ejecuci&oacute; que implementa la solcui&oaucute;n al problema
	 * del consenso. Cada hilo representa a uno de los procesos contendentes.
	 * @author <a href="mailto:manuel_castillo_cc@ciencias.unam.mx" >Manuel
	 * "Nachintoch" Castillo</a>
	 * @version 1.0, november 2015
	 * @since Consensus Example 1.0, november 2015
	 */
    private static class Process implements Callable<Boolean> {

		// atributos de clase

		/**
		 * Identificador del hilo dentro de la soluci&oacute;n al problema del
		 * consenso.
		 * @since Process 1.0, november 2015
		 */
        private int i;
        
        /**
		 * Registro at&oacute;mico usado para contener una decisi&oacute;n
		 * temporal.
		 * @since Process 1.0, november 2015
		 */
        private AtomicBoolean decisioni;
        
        /**
		 * Registro at&oacute;mico usado para contener el valor de la entrada
		 * del hilo.
		 * @since Process 1.0, november 2015
		 */
        private AtomicBoolean ini;

		// método constructor

		/**
		 * Construye un hilo que competir&aacute; por decidir el consenso.
		 * @param i - El identificador num&eacute;rico del hilo.
		 * @param input - La entrada sobre la que va a decidir el hilo.
		 * @since Process 1.0, november 2015
		 */
        private Process(int i, boolean input) {
            this.i = i;
            ini = new AtomicBoolean(input);
            decisioni = new AtomicBoolean(false);
        }//constructor

		// métodos de implementación

        @Override
        public Boolean call() {
            try {
		        consensus();
		    } catch(InterruptedException e) {
		        Logger.getLogger(Process.class.getName())
					.log(Level.SEVERE, null, e);
				decision.set(false);
		    }
            return decision.get();
        }//call

		// métodos auxiliares

		/**
		 * Resuelve el problema del consenso.
		 * @throws InterruptedException - Si esperando por apropiarse del
		 * sem&aacute;foro el hilo es interrumpido.
		 * @since Process 1.0, november 2015
		 */
        private void consensus() throws InterruptedException {
        	if(i == 1 || i == 2) {
        		mutex.acquire();
                if(x.get()) {
                    decisioni.set(true);
                    mutex.release();
                } else {
                    x.set(ini.get());
                    mutex.release();
                    mutex.acquire();
                    if(!y.get()) {
                    	y.set(true);
                    	decisioni.set(ini.get());
                    	mutex.release();
                    } else {
                        mutex.release();
                        if(!x.get()) {
                            decisioni.set(false);
                        }//decide si x había sido cambiado
                    }//decide dependiendo si y ha sido cambiado
                }//decide dependiendo si x ha sido cambiado
                decision.set(decisioni.get());
                finish.set(true);
            } else {
                while(!finish.get()) {
                	Thread.sleep(100);
                }//espera por los distinguidos
            }//los procesos deciden o esperan si son o no distinguidos
        }//consensus

    }//Process Callable

}//ConsensusExample class


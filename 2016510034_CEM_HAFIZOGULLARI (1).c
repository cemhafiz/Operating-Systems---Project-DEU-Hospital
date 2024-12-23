#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 

#define REGISTRATION_SIZE 10
#define RESTROOM_SIZE 10
#define CAFE_NUMBER 10
#define GP_NUMBER 10
#define PHARMACY_NUMBER 10
#define BLOOD_LAB_NUMBER 10
#define OR_NUMBER 10
#define SURGEON_NUMBER 30
#define NURSE_NUMBER 30
#define SURGEON_LIMIT 5
#define NURSE_LIMIT 5
#define PATIENT_NUMBER 1000
#define HUNGER_INCREASE_RATE_LIMIT  10
#define RESTROOM_INCREASE_RATE_LIMIT  10
#define REGISTRATION_COST 100
#define PHARMACY_COST 200
#define BLOOD_LAB_COST 200
#define SURGERY_OR_COST 200
#define SURGEON_COST 100
#define NURSE_COST 50
#define CAFE_COST_LIMIT 200
#define WAIT_TIME_LIMIT 100
#define REGISTRATION_TIME_LIMIT 100
#define GP_TIME_LIMIT 200
#define PHARMACY_TIME_LIMIT 100
#define BLOODLAB_TIME_LIMIT 200
#define SURGERY_TIME_LIMIT 500
#define CAFE_TIME_LIMIT 100
#define RESTROOM_TIME_LIMIT 100

void *patient(void *num);
void *registration_function(void * num);
void *restroom_function(void * num);
void *cafe_function(void * num);
void *gp_function(void * num);
void *pharmacy_function(void * num);
void *bloodlab_function(void * num);
void *operation_function(void * num);

void pickRegistrationOffice();
void pickRestroom();
void pickCafeCashier();
void pickGP();
void pickPharmacyCashier();
void pickBloodLabAssistant();
void pickOR();
void pickSurgeon();
void pickNurse();

sem_t registration_sem[REGISTRATION_SIZE];
sem_t restroom_sem[RESTROOM_SIZE];
sem_t cafe_sem[CAFE_NUMBER];
sem_t gp_sem[GP_NUMBER];
sem_t pharmacy_sem[PHARMACY_NUMBER];
sem_t bloodlab_sem[BLOOD_LAB_NUMBER];
sem_t or_sem[OR_NUMBER];
sem_t surgeon_sem[SURGEON_NUMBER];
sem_t nurse_sem[NURSE_NUMBER];
sem_t wakeRegistration;
sem_t wakeRestroom;
sem_t wakeCafe;
sem_t wakeGP;
sem_t wakePharmacy;
sem_t wakeBloodLab;
sem_t wakeOR;
sem_t wakeSurgeon;
sem_t wakeNurse;
sem_t wakePatient;

static int selectedRegistrationOffice;//represent Registration Office
static int control_Registration;// check the status of the next Registration Office number and assign its value to selectedRegistrationOffice if appropriate. 
static int selectedRestroom;//represent Restroom
static int control_Restroom;// check the status of the next Restroom number and assign its value to selectedRestroom if appropriate. 
static int selectedCafeCashier;//represent Cafe
static int control_Cafe;// check the status of the next Cafe number and assign its value to selectedCafeCashier if appropriate. 
static int selectedGPOffice;
static int control_GP;// check the status of the next GP Office Number and assign its value to selectedGPOffice if appropriate. 
static int selectedPharmacyCashier;
static int control_Pharmacy;// check the status of the next Pharmacy Cashier and assign its value to selectedPharmacyCashier if appropriate. 
static int selectedbloodlab_semtation;
static int control_BloodLab;// check the status of the next Blood Lab Station and assign its value to selectedbloodlab_semtation if appropriate. 
static int selectedOR;
static int control_OR; 
static int selectedSurgeonforOR[SURGEON_LIMIT];
static int control_SurgeonforOR; 
static int selectedNurseforOR[NURSE_LIMIT];
static int control_NurseforOR;

int Registration_allfinished = 0;
int Registrationvalue[REGISTRATION_SIZE];
int Restroom_allfinished = 0;
int Restroomvalue[RESTROOM_SIZE];
int Cafe_allfinished = 0;
int Cafevalue[CAFE_NUMBER];
int GP_allfinished = 0;
int GPvalue[GP_NUMBER];
int Pharmacy_allfinished = 0;
int Pharmacyvalue[PHARMACY_NUMBER];
int BloodLab_allfinished = 0;
int BloodLabvalue[BLOOD_LAB_NUMBER];
int OR_allfinished = 0;
int ORvalue[OR_NUMBER];
int Surgeon_allfinished = 0;
int Surgeonvalue[SURGEON_NUMBER];
int Nurse_allfinished = 0;
int Nursevalue[NURSE_NUMBER];

void *patient(void * num)
{
	int number = *(int *)num;
    int randomtime = rand()%WAIT_TIME_LIMIT;
	
	sem_wait(&wakePatient);
	printf("Patient %d arrived at hospital.\n", number);
    printf("Patient %d came to Registration Office\n", number);
	sleep(1);
    
	sem_post(&wakeRegistration);
    sleep(randomtime);
	sem_post(&wakePatient);
}

void *registrationfunc(void * num)
{	
	while(!Registration_allfinished)
	{
		sem_wait(&wakeRegistration);
		if(!Registration_allfinished)
		{	
			sem_wait(&registration_sem[selectedRegistrationOffice]);
            
            sem_getvalue(&registration_sem[selectedRegistrationOffice], &Registrationvalue[selectedRegistrationOffice]);
            printf("%d\n", selectedRegistrationOffice);
            printf("Registration Office %d is %d waiting for Patients.\n", selectedRegistrationOffice, Registrationvalue[selectedRegistrationOffice]);
            int oldRegistrationOffice=selectedRegistrationOffice;
            if (Registrationvalue[selectedRegistrationOffice] == 0)
            {
                pickRegistrationOffice();
                printf("Patient is talking with Office Attandent to be forwarded %d\n",oldRegistrationOffice);
                int number = *(int *)num;
    			int randomtime = rand()%REGISTRATION_TIME_LIMIT;
                int l;
                for(l = 0; l < 1; l++)
                {
                    sem_post(&registration_sem[oldRegistrationOffice]);
                }
                printf("Patient leaving Registration Office %d\n",oldRegistrationOffice);
            }
		}
	}
}
/*void *restroomfunc(void * num)
{	
	while(!Restroom_allfinished)
	{
		sem_wait(&wakeRestroom);
		if(!Restroom_allfinished)
		{	
			sem_wait(&restroom_sem[selectedRestroom]);
            
            sem_getvalue(&restroom_sem[selectedRestroom],&Restroomvalue[selectedRestroom]);
            printf("%d\n",selectedRestroom);
            printf("Restroom %d is %d waiting for Patients.\n", selectedRestroom,Restroomvalue[selectedRestroom]);
             int oldRestroom = selectedRestroom;
            if (Restroomvalue[selectedRestroom] == 0)
            {
                
                pickRestroom();
                printf("Patient is resting in the Restroom %d\n",oldRestroom);
                sleep(1);
                int l;
                for(l = 0; l < 1; l++)
                {
                    sem_post(&restroom_sem[oldRestroom]);
                                
                }
                printf("Patient leaving Restroom %d\n",oldRestroom);
            }
            
		}
			
	}

}*/

void pickRegistrationOffice()
{
	int registrationno = -1;
	while (registrationno == -1)
	{
			control_Registration++;
			sem_getvalue(&registration_sem[control_Registration%REGISTRATION_SIZE],&Registrationvalue[control_Registration%REGISTRATION_SIZE]);
			
			if (Registrationvalue[control_Registration%REGISTRATION_SIZE]>=0)
			{
				sem_wait(&wakeRegistration);
				selectedRegistrationOffice = control_Registration%REGISTRATION_SIZE;
				registrationno = 1;
				sem_post(&wakeRegistration);
			}
	}
}

int main(int argc, char *argv[]){

    srand(time(NULL));

    int hospital_wallet = 0;
    int i;

    pthread_t patient_tid[PATIENT_NUMBER];
    pthread_t registration_tid[REGISTRATION_SIZE];
	pthread_t restroom_tid[RESTROOM_SIZE];
    pthread_t cafe_tid[CAFE_NUMBER];
	pthread_t gp_tid[GP_NUMBER];
    pthread_t pharmacy_tid[PHARMACY_NUMBER];
	pthread_t blood_lab_tid[BLOOD_LAB_NUMBER];
    pthread_t or_tid[OR_NUMBER];
	pthread_t surgeon_tid[SURGEON_NUMBER];
    pthread_t nurse_tid[NURSE_NUMBER];
    
    int hunger_meter[PATIENT_NUMBER];
    int restroom_meter[PATIENT_NUMBER];
    
    int NumberRegistration[REGISTRATION_SIZE];
    int NumberCafe[CAFE_NUMBER];
    int NumberRestroom[RESTROOM_SIZE];
    int NumberGP[GP_NUMBER];
    int NumberPharmacy[PHARMACY_NUMBER];
    int NumberBloodLab[BLOOD_LAB_NUMBER];
	int NumberPatient[PATIENT_NUMBER];
    int NumberSurgeon[SURGEON_NUMBER];
    int NumberNurse[NURSE_NUMBER];
    int NumberOR[OR_NUMBER];
    int NumberOR_SURGEON[SURGEON_LIMIT];
    int NumberOR_NURSE[NURSE_LIMIT];
    
    int random_wait_time = rand()%WAIT_TIME_LIMIT;
    int random_surgeon_wait = rand()%SURGEON_LIMIT;
    int random_nurse_wait = rand()%NURSE_LIMIT;
    int random_registration_wait = rand()%REGISTRATION_SIZE;
    int random_hunger_wait = rand()%HUNGER_INCREASE_RATE_LIMIT;
    int random_restroom_wait = rand()%RESTROOM_INCREASE_RATE_LIMIT;

    selectedRegistrationOffice = random_registration_wait;
    control_Registration = selectedRegistrationOffice;

    for (i = 0; i < PATIENT_NUMBER; i++) 
	{
		NumberPatient[i] = i;
	}
    for (i = 0; i < REGISTRATION_SIZE; i++) 
	{
		NumberRegistration[i] = i;
	}
    for (i = 0; i < RESTROOM_SIZE; i++) 
	{
		NumberRestroom[i] = i;
	}
    for (i = 0; i < CAFE_NUMBER; i++) 
	{
		NumberCafe[i] = i;
	}
    for (i = 0; i < GP_NUMBER; i++) 
	{
		NumberGP[i] = i;
	}
    for (i = 0; i < PHARMACY_NUMBER; i++) 
	{
		NumberPharmacy[i] = i;
	}
    for (i = 0; i < BLOOD_LAB_NUMBER; i++) 
	{
		NumberBloodLab[i] = i;
	}
    for (i = 0; i < SURGEON_NUMBER; i++) 
	{
		NumberSurgeon[i] = i;
	}
    for (i = 0; i < NURSE_NUMBER; i++) 
	{
		NumberNurse[i] = i;
	}
    for (i = 0; i < OR_NUMBER; i++) 
	{
		NumberOR[i] = i;
	}

	sem_init(&wakeRegistration, 0, 0);
	sem_init(&wakePatient, 0, PATIENT_NUMBER);

    for (i = 0; i < REGISTRATION_SIZE; i++)
	{
		sem_init(&registration_sem[i], 0, 0);
	}
    for (i = 0; i < RESTROOM_SIZE; i++)
	{
		sem_init(&restroom_sem[i], 0, 0);
	}
    for (i = 0; i < CAFE_NUMBER; i++)
	{
		sem_init(&cafe_sem[i], 0, 0);
	}
    for (i = 0; i < GP_NUMBER; i++)
	{
		sem_init(&gp_sem[i], 0, 0);
	}
    for (i = 0; i < PHARMACY_NUMBER; i++)
	{
		sem_init(&pharmacy_sem[i], 0, 0);
	}
    for (i = 0; i < BLOOD_LAB_NUMBER; i++)
	{
		sem_init(&bloodlab_sem[i], 0, 0);
	}
    for (i = 0; i < SURGEON_NUMBER; i++)
	{
		sem_init(&surgeon_sem[i], 0, 0);
	}
    for (i = 0; i < NURSE_NUMBER; i++)
	{
		sem_init(&nurse_sem[i], 0, 0);
	}
    for (i = 0; i < OR_NUMBER; i++)
	{
		sem_init(&or_sem[i], 0, 0);
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (i = 0; i < PATIENT_NUMBER; i++)
	{
		pthread_create(&patient_tid[i], NULL, patient, (void *)&NumberPatient[i]);
		sleep(1);
	}
    
    for (i = 0; i < PATIENT_NUMBER; i++)
	{
		pthread_join(patient_tid[i], NULL);
	}

    Registration_allfinished = 1;

    for ( i = 0; i < PATIENT_NUMBER/REGISTRATION_SIZE; i++)//process repetitions
    {
        sem_post(&wakeRegistration);
    }

    for (i = 0; i < REGISTRATION_SIZE; i++)
    {
        pthread_join(registration_tid[i], NULL);
    }
    
    printf("Hospital Closed\n");
    return(0);
}
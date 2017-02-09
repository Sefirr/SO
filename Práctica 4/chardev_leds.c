/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for copy_to_user */
#include <asm/io.h>

/*  
 *  Prototypes 
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80		/* Max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;		/* This will be initialized every time the 
				   device is opened successfully */ 
static int counter=0;		/* Tracks the number of times the character
				 * device has been opened */

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'sudo mknod -m 666 /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(Major, DEVICE_NAME);
}

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/chardev"
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;

	/* Initialize msg */
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);

	/* Initially, this points to the beginning of the message */
	msg_Ptr = msg;

	/* Increase the module's reference counter */
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/*
	 * Number of bytes actually written to the buffer 
	 */
	int bytes_to_read = length;

	/*
	 * If we're at the end of the message, 
	 * return 0 -> end of file 
	 */
	if (*msg_Ptr == 0)
		return 0;

	/* Make sure we don't read more chars than 
	 * those remaining to read
    	 */
	if (bytes_to_read > strlen(msg_Ptr))
		bytes_to_read=strlen(msg_Ptr);
			
	/* 
	 * Actually transfer the data onto the userspace buffer. 
	 * For this task we use copy_to_user() due to security issues 
	 */
	if (copy_to_user(buffer,msg_Ptr,bytes_to_read))
		return -EFAULT;

	/* Update the pointer for the next read operation */	
	msg_Ptr+=bytes_to_read;	
	
	/* 
	 * The read operation returns the actual number of bytes 
	 * we copied  in the user's buffer
	 */
	return bytes_to_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/chardev 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	/*
	 * Number of bytes actually read to the buffer 
	 */
	int bytes_to_write = len;
	int retries = 5;
	int timeout = 1000;
	int state;
	char kbuf[len];
	bool tecla[3] = {0};
	int i;
	bool configuracion_correcta = true;
	
	// Comprobación de que se ha copiado correctamente al buffer
	if(copy_from_user(kbuf,buff+'\0',len))
	   return -EINVAL;
	
	// Comprobación de que lo que se ha introducido al escribir es una buena configuración de los leds
	


	// Tratamiento de los leds para configurarlos en función de lo que escriba el usuario.
	state = 0x00;

	for(i = 0; i < len; i++){
	   if((kbuf[i] == '1') && (!tecla[0])){
	     	state += 0x02;
		tecla[0] = true;
	   }
	   if((kbuf[i] == '2') && (!tecla[1])){
	      state += 0x04;
	      tecla[1] = true;
	   }
	   if((kbuf[i] == '3') && (!tecla[2])){
	      state += 0x01;		
	      tecla[2] = true;
	   } 
	}
	
	outb(0xed,0x60); // Le decimos al teclado que queremos modificar los leds
	udelay(timeout);

	while(retries!=0 && inb(0x60)!=0xfa) { // esperamos al controlador
	   retries--;
	   udelay(timeout);
	}

	if(retries!=0) { // comprobamos que el teclado esta listo
	   outb(state,0x60);
	}
 	udelay(timeout);
	return bytes_to_write;

}

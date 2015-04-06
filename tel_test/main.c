/*
 *    This application query the available bluetooth devices, then selecting
 *  the one with the name "Én" (my mobile phone). After this an RFCOMM
 *  connection is established between the two devices (my mobile and RPi), and
 *  finally i try to read some data from my mobile just to give me enough time
 *  to check (on my mobile screen header part) if the connection has been really
 *  set up. (My mobile will be also referred as "remote device".)
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

typedef enum {false, true} bool;

int main()
{
    // dev_id:   Device ID of BT adapter in RPi.
    // sock_inner:
    //           Socket for communication between RPi and BT adapter.
    // dev_nums: Number of BT-available devices.
    // sock_out: Socket for comm. between adapter and remote device.
    // status:   Indicates if the connection to the remote device was successful
    //           or not
    int dev_id, sock_inner, dev_nums, sock_out, status;
    // addr:     BT address of my mobile.
    // name:     The name ("Én") of my mobile.
    // read_buf: A comm. buffer between BT adapter and remote device.
    char addr[19] = "";
    char name[248] = "";
    char read_buf[128] = "";
    // ii:       List of data about the available BT devices.
    inquiry_info *ii = NULL;
    // not_available:
    //           False if my mobile can be found amongst the available
    //           BT devices.
    bool not_available = true;
    // dest_addr:
    //           Informations about the "sock_out" socket.
    struct sockaddr_rc dest_addr = {0};


    /*    Passing NULL as a parameter (bdaddr_t *) results in retrieving the
     *  first (and in this case the only) bluetooth adapter's identifier.
     *    Alternate method is to hardcode the actual BT device address. In
     *  this case: "5C:F3:70:65:B7:7D"
     */
//    dev_id = hci_get_route(NULL);
    dev_id = hci_devid("5C:F3:70:65:B7:7D");
    if (dev_id < 0) {
        printf("Invalid or missing device.");
        return 0;
    }

    // Opening the socket.
    sock_inner = hci_open_dev(dev_id);
    if (sock_inner < 0) {
        printf("Unable to open socket.");
        return 0;
    }

    /*
     *    Initializing ii with a maximum of 255 element (default value for an
     *  10 sec scan).
     *    Perform the actual scan.
     */
    ii = (inquiry_info*)malloc(255 * sizeof(inquiry_info));
    dev_nums = hci_inquiry(dev_id, 8, 255, NULL, &ii, IREQ_CACHE_FLUSH);

    /*
     *    Printing to console information about the available BT devices
     *  (equivalent of "hcitool scan") until it finds my mobile.
     */
    printf("A not necessarily complete list of available devices:\n");
    for (;(dev_nums >= 0) && (not_available); dev_nums--) {
        ba2str(&(ii+dev_nums)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        hci_read_remote_name(sock_inner, &(ii+dev_nums)->bdaddr,
                             sizeof(name), name, 0);
        printf("\t%s  %s\n", addr, name);
        if (strcmp(name, "Én") == 0)
            not_available = false;
    }
    // Handle if the BT is turned off on my mobile.
    if(not_available) {
        printf("\nThe desired device is not available. =(\n");
        free(ii);
        close(sock_inner);
        return 0;
    }

    // Try to establish an RFCOMM connection between BT adapter and my mobile.
    printf("Trying to connect to the device \"%s\" (%s)\n", name, addr);
    // Configuring the socket between BT adapter and remote device.
    sock_out = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    dest_addr.rc_family = AF_BLUETOOTH;
    str2ba(addr, &dest_addr.rc_bdaddr);
    dest_addr.rc_channel = (uint8_t) 19;

    // Connecting to the remote device. (eq. of "hcitool connect ...")
    status = connect(sock_out, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    /*
     *    Handle if the connection cannot be established.
     *    Wait 5 secs to get enough time for connection check.
     */
    if (status == 0) {
        printf("\nTest the connection on your mobile!\n");
        int timed_waiting = 5;
        for (;timed_waiting > 0; timed_waiting--) {
            printf("%i\n", timed_waiting);
            fflush(stdout);
            sleep(1);
        }
    } else {
        printf("Cannot establish connection between BT adapter and remote device.\n");
    }

    // Cleaning.
    close(sock_out);
    free(ii);
    close(sock_inner);


    return 0;
}

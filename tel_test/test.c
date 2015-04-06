//#include <stdio.h>
//#include <stdlib.h>
//
//#include <sys/socket.h>
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/hci_lib.h>
//
//int main()
//{
//    int dev_id, sock, dev_nums;
//    char addr[19] = { 0 };
//    char name[248] = { 0 };
//
//
//    /*    Passing NULL as a parameter (bdaddr_t *) results in retrieving the
//     *  first (and in this case the only) bluetooth adapter's identifier.
//     *    Alternate method is to hardcode the actual BT device address. In
//     *  this case: "5C:F3:70:65:B7:7D"
//     */
////    dev_id = hci_get_route(NULL);
//    dev_id = hci_devid("5C:F3:70:65:B7:7D");
//
//    sock = hci_open_dev(dev_id);
//
//    inquiry_info *ii = (inquiry_info*)malloc(255 * sizeof(inquiry_info));
//    dev_nums = hci_inquiry(dev_id, 8, 255, NULL, &ii, IREQ_CACHE_FLUSH);
//
//    printf("Available devices:\n");
//    for (;dev_nums >= 0; dev_nums--) {
//        ba2str(&(ii+dev_nums)->bdaddr, addr);
//        memset(name, 0, sizeof(name));
//        hci_read_remote_name(sock, &(ii+dev_nums)->bdaddr, sizeof(name), name, 0);
//        printf("\t%s  %s\n", addr, name);
//    }
//
//    free(ii);
//    close(sock);
//
//    printf("Hello world!\n");
//    return 0;
//}

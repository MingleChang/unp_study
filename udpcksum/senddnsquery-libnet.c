static libnet_t *l;

void open_output(void) {
  char errbuf[LIBNET_ERRBUF_SIZE];
  l = libnet_init(LIBNET_RAW4, NULL, errbuf);
  if (l == NULL) {
    err_quit("Can't initialize libnet: %s", errbuf);
  }
}

void send_dns_query(void) {
  char qbuf[24], *ptr;
  u_int16_t one;
  int packet_size = LIBNET_UDP_H + LIBNET_DNSV4_H + 24;
  static libnet_ptag_t ip_tag, udp_tag, dns_tag;
  ptr = qbuf;
  memcpy(ptr, "\001a\014root-servers\003net\000", 20);
  ptr += 20;
  one = htons(1);
  memcpy(ptr, &one, 2);
  ptr += 2;
  memcpy(ptr, &one, 2);

  dns_tag = libnet_build_dnsv4(1234, 0x0100, 1, 0, 0, qbuf, 24, l, dns_tag);
  udp_tag = libnet_build_udp(((struct sockaddr_in *)local)->sin_port, ((struct sockaddr_in *)dest)->sin_port, packet_size, 0, NULL, 0, l, udp_tag);
  if (zerosum) {
    if (libnet_toggle_checksum(l, udp_tag, LIBNET_OFF) < 0) {
      err_quit("turning off checksums: %s\n", libnet_geterror(l));
    }
  }
  ip_tag = libnet_build_ipv4(packet_size + LIBNET_IPV4_H, 0, 0, 0, TTL_OUT, 0, ((struct sockaddr_in *)local)->sin_addr.s_addr, ((struct sockaddr_in *)dest)->sin_addr.s_addr, NULL, 0, l, ip_tag);
  if (libnet_write(l) < 0) {
    err_quit("libnet_write: %s\n", libnet_geterror(l));
  }
  if (verbose) {
    printf("sent: %d bytes of data\n", packet_size);
  }
}

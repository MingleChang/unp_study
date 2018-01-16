void sadb_add(struct sockaddr *src, struct sockaddr *dst, int type, int alg, int spi, int keybits, unsigned char *keydata) {
  int s;
  char *buf[4096], *p;
  struct sadb_msg *msg;
  struct sadb_sa *saext;
  struct sadb_address *addrext;
  struct sadb_key *keyext;
  int len;
  int mypid;

  s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);

  mypid = getpid();

  bzero(&buf, sizeof(buf));
  p = buf;
  msg = (struct sadb_msg *)p;
  msg->sadb_msg_version = PF_KEY_V2;
  msg->sadb_msg_type = SADB_ADD;
  msg->sadb_msg_satype = type;
  msg->sadb_msg_pid = getpid();
  len = sizeof(*msg);
  p += sizeof(*msg);

  saext = (struct sadb_sa *)p;
  saext->sadb_sa_len = sizeof(*saext) / 8;
  saext->sadb_sa_exttype = SADB_EXT_SA;
  saext->sadb_sa_spi = htonl(spi);
  saext->sadb_sa_replay = 0;
  saext->sadb_sa_state = SADB_SASTATE_MATURE;
  saext->sadb_sa_auth = alg;
  saext->sadb_sa_encrypt = SADB_EALG_NONE;
  saext->sadb_sa_flags = 0;
  len += saext->sadb_sa_len * 8;
  p += saext->sadb_sa_len * 8;

  addrext = (struct sadb_address *)p;
  addrext->sadb_address_len = (sizeof(*addrext) + salen(src) + 7) / 8;
  addrext->sadb_address_exttype = SADB_EXT_ADDRESS_SRC;
  addrext->sadb_address_proto = 0;
  addrext->sadb_address_prefixlen = prefix_all(src);
  addrext->sadb_address_reserved = 0;
  memcpy(addrext + 1, src, salen(src));
  len += addrext->sadb_address_len * 8;
  p += addrext->sadb_address_len * 8;

  addrext = (struct sadb_address *)p;
  addrext->sadb_address_len = (sizeof(*addrext) + salen(dst) + 7) / 8;
  addrext->sadb_address_exttype = SADB_EXT_ADDRESS_DST;
  addrext->sadb_address_proto = 0;
  addrext->sadb_address_prefixlen = prefix_all(dst);
  addrext->sadb_address_reserved = 0;
  memcpy(addrext + 1, dst, salen(dst));
  len += addrext->sadb_address_len * 8;
  p += addrext->sadb_address_len * 8;

  keyext = (struct sadb_key *)p;
  keyext->sadb_key_len = (sizeof(*keyext) + (keybits / 8) + 8) / 8;
  keyext->sadb_key_exttype = SADB_EXT_KEY_AUTH;
  keyext->sadb_key_bits = keybits;
  keyext->sadb_key_reserved = 0;
  memcpy(keyext + 1, keydata, keybits / 8);
  len += keyext->sadb_key_len * 8;
  p += keyext->sadb_key_len * 8;

  msg->sadb_msg_len = len / 8;
  printf("Sending add message:\n");
  print_sadb_msg(buf, len);
  Write(s, buf, len);

  printf("\nReply returned:\n");
  for (;;) {
    int msglen;
    struct sadb_msg *msgp;

    msglen = Read(s, &buf, sizeof(buf));
    msgp = (struct sadb_msg *)&buf;
    if (msgp->sadb_msg_pid == mypid && msgp->sadb_msg_type == SADB_ADD) {
      print_sadb_msg(msgp, msglen);
      break;
    }
  }
  close(s);
}

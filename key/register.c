void sadb_register(int type) {
  int s;
  char buf[4096];
  struct sadb_msg msg;
  int goteof;
  int mypid;

  s = Socket(PF_KEY, SOCK_RAW, PF_KEY_V2);

  mypid = getpid();

  bzero(&msg, sizeof(msg));
  msg.sadb_msg_version = PF_KEY_V2;
  msg.sadb_msg_type = SADB_REGISTER;
  msg.sadb_msg_satype = type;
  msg.sadb_msg_len = sizeof(msg) / 8;
  msg.sadb_msg_pid = mypid;
  printf("Sending register message:\n");
  print_sadb_msg(&msg, sizeof(msg));
  Write(s, &msg, sizeof(msg));

  printf("\nReply returned:\n");
  for (;;) {
    int msglen;
    struct sadb_msg *msgp;

    msglen = Read(s, &buf, sizeof(buf));
    msgp = (struct sadb_msg *)&buf;
    if (msgp->sadb_msg_pid == mypid && msgp->sadb_msg_type == SADB_REGISTER) {
      print_sadb_msg(msgp, msglen);
      break;
    }
  }
  close(s);
}

/*-
 * Copyright (c) 2017 Lawrence Esswood
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "cheric.h"
#include "unistd.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"

#define BIG_SIZE 0x1000

char source[BIG_SIZE];
char dest[BIG_SIZE];

int main(register_t arg, capability carg) {

    while(try_get_fs() == NULL) {
        sleep(0);
    }

    printf("Fs test start..\n");

    FILE_t file = open("foobar", 1, 1, MSG_NONE);

    assert(file != NULL);

    char buf[100];

    const char message[] = "Hello World! Yay a message!\n";
    size_t ms = sizeof(message);

    ssize_t result;

    result = write(file, message, ms);

    assert_int_ex(result, ==, ms);

    result = lseek(file, 0, SEEK_SET);

    assert_int_ex(result, ==, 0);

    result = read(file, buf, ms);

    assert_int_ex(result, ==, ms);

    result = strcmp(buf, message);

    assert_int_ex(result, == , 0);

    result = close(file);

    assert_int_ex(result, ==, 0);

    for(size_t i = 0; i < BIG_SIZE; i++) {
        source[i] = 'A';
    }

    file = open("bigtest", 1, 1, MSG_NONE);

    assert(file != NULL);

    result = write(file, source, BIG_SIZE);

    assert_int_ex(-result, ==, -BIG_SIZE);

    result = lseek(file, 0, SEEK_SET);
    assert_int_ex(result, ==, 0);

    result = read(file, dest, BIG_SIZE);
    assert_int_ex(result, ==, BIG_SIZE);

    for(size_t i = 0; i < BIG_SIZE; i++) {
        assert_int_ex('A', ==, dest[i]);
        dest[i] = 0;
    }

    FILE_t file2 = open("Target", 1, 1, MSG_NONE);

    result = lseek(file, 0, SEEK_SET);
    assert_int_ex(result, ==, 0);

    result = sendfile(file2, file, BIG_SIZE);
    assert_int_ex(result, ==, BIG_SIZE);

    flush(file);

    result = close(file);
    assert_int_ex(result, ==, 0);

    flush(file2);

    result = lseek(file2, 0, SEEK_SET);
    assert_int_ex(result, ==, 0);

    result = read(file2, dest, BIG_SIZE);
    assert_int_ex(result, ==, BIG_SIZE);

    for(size_t i = 0; i < BIG_SIZE; i++) {
        assert_int_ex('A', ==, dest[i]);
    }

    result = close(file2);
    assert_int_ex(result, ==, 0);

    printf("Fs test success!\n");
}

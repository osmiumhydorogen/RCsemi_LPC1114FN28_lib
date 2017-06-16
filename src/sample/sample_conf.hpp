/*
 * sample_conf.hpp
 *
 *  Created on: 2017/03/06
 *      Author: osmium
 */

#ifndef SAMPLE_SAMPLE_CONF_HPP_
#define SAMPLE_SAMPLE_CONF_HPP_

//サンプルを実行したければコメントアウトを外す
#define BUILD_4_SAMPLE

#ifdef BUILD_4_SAMPLE
#define MOTOR_SAMPLE

//#define PSPAD_SAMPLE //[W.I.P]

//#define UART_PORT_SAMPLE //[W.I.P]

//#define IM315_SAMPLE
#endif

#endif /* SAMPLE_SAMPLE_CONF_HPP_ */
